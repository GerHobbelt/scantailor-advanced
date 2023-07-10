/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2015  Joseph Artsimovich <joseph.artsimovich@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Transpose.h"
#include "Utils.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace opencl
{

OpenCLGrid<float> transpose(
	cl::CommandQueue const& command_queue, cl::Program const& program,
	OpenCLGrid<float> const& src_grid, int dst_padding,
	std::vector<cl::Event> const* dependencies,
	std::vector<cl::Event>* completion_set)
{
	cl::Context const context = command_queue.getInfo<CL_QUEUE_CONTEXT>();

	cl::Buffer dst_buffer(
		context, CL_MEM_READ_WRITE, src_grid.totalBytesWithDifferentPadding(dst_padding)
	);
	OpenCLGrid<float> dst_grid(dst_buffer, src_grid.height(), src_grid.width(), dst_padding);

	transpose(command_queue, program, src_grid, dst_grid, dependencies, completion_set);

	return dst_grid;
}

void transpose(
	cl::CommandQueue const& command_queue, cl::Program const& program,
	OpenCLGrid<float> const& src_grid, OpenCLGrid<float>& dst_grid,
	std::vector<cl::Event> const* dependencies,
	std::vector<cl::Event>* completion_set)
{
	assert(src_grid.width() == dst_grid.height());
	assert(src_grid.height() == dst_grid.width());

	cl::Device const device = command_queue.getInfo<CL_QUEUE_DEVICE>();
	size_t const max_wg_size = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
	size_t const cacheline_size = device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE>();
	uint64_t const local_mem_size = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();

	size_t tile_dim = std::min<size_t>(cacheline_size/sizeof(float), max_wg_size);
	while ((tile_dim + 1) * tile_dim * sizeof(float) > local_mem_size) {
		tile_dim >>= 1;
	}

	size_t const h_wg_size = std::min<size_t>(tile_dim, max_wg_size);
	size_t v_wg_size = std::min<size_t>(tile_dim, max_wg_size / h_wg_size);

	cl::Kernel kernel(program, "transpose_float_grid");
	int idx = 0;
	kernel.setArg(idx++, src_grid.width());
	kernel.setArg(idx++, src_grid.height());
	kernel.setArg(idx++, src_grid.buffer());
	kernel.setArg(idx++, src_grid.offset());
	kernel.setArg(idx++, src_grid.stride());
	kernel.setArg(idx++, dst_grid.buffer());
	kernel.setArg(idx++, dst_grid.offset());
	kernel.setArg(idx++, dst_grid.stride());
	kernel.setArg(idx++, cl_int(tile_dim));
	kernel.setArg(idx++, cl::Local((tile_dim + 1) * tile_dim * sizeof(float)));
	kernel.setArg(idx++, cl_int(tile_dim + 1));

	cl::Event evt;
	command_queue.enqueueNDRangeKernel(
		kernel,
		cl::NullRange,
		cl::NDRange(
			h_wg_size * ((src_grid.width() + tile_dim - 1) / tile_dim),
			v_wg_size * ((src_grid.height() + tile_dim - 1) / tile_dim)
		),
		cl::NDRange(h_wg_size, v_wg_size),
		dependencies,
		&evt
	);
	indicateCompletion(completion_set, evt);
}

} // namespace opencl
