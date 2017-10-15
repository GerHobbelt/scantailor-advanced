/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C)  Joseph Artsimovich <joseph.artsimovich@gmail.com>

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

#include "OrderByWidthProvider.h"
#include "Params.h"
#include "PageId.h"
#include <QSizeF>
#include <memory>
#include <assert.h>

namespace select_content
{

OrderByWidthProvider::OrderByWidthProvider(IntrusivePtr<Settings> const& settings)
:	m_ptrSettings(settings)
{
}

bool
OrderByWidthProvider::precedes(
	PageId const& lhs_page, bool const lhs_incomplete,
	PageId const& rhs_page, bool const rhs_incomplete) const
{

    if (lhs_incomplete != rhs_incomplete) {
        // Pages with question mark go to the bottom.
        return rhs_incomplete;
    } else if (lhs_incomplete) {
        assert(rhs_incomplete);
        // Two pages with question marks are ordered naturally.
        return lhs_page < rhs_page;
    }

    assert(lhs_incomplete == false);
    assert(rhs_incomplete == false);

	std::auto_ptr<Params> const lhs_params(m_ptrSettings->getPageParams(lhs_page));
	std::auto_ptr<Params> const rhs_params(m_ptrSettings->getPageParams(rhs_page));
	
	QSizeF lhs_size;
	if (lhs_params.get()) {
        lhs_size = lhs_params->contentRect().size();
	}
	QSizeF rhs_size;
	if (rhs_params.get()) {
        rhs_size = rhs_params->contentRect().size();
	}
	
    bool const lhs_invalid = !lhs_size.isValid();
    bool const rhs_invalid = !rhs_size.isValid();

    if (lhs_invalid != rhs_invalid) {
        // Pages with question mark go to the bottom.
        return rhs_invalid;
    } else if (lhs_invalid) {
        assert(rhs_invalid);
        // Two pages with question marks are ordered naturally.
        return lhs_page < rhs_page;
    }

    assert(lhs_invalid == false);
    assert(rhs_incomplete == false);

    if (lhs_size.width() != rhs_size.width()) {
        return lhs_size.width() < rhs_size.width();
    } else {
        return lhs_page < rhs_page;
    }
}

} // namespace select_content
