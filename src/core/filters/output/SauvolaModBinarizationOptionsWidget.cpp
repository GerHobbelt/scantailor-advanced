// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "SauvolaModBinarizationOptionsWidget.h"

#include <utility>


namespace output {

SauvolaModBinarizationOptionsWidget::SauvolaModBinarizationOptionsWidget(std::shared_ptr<Settings> settings)
    : m_settings(std::move(settings)),
      m_connectionManager(std::bind(&SauvolaModBinarizationOptionsWidget::setupUiConnections, this)) {
  setupUi(this);

  m_delayedStateChanger.setSingleShot(true);

  setupUiConnections();
}

void SauvolaModBinarizationOptionsWidget::updateUi(const PageId& pageId) {
  auto block = m_connectionManager.getScopedBlock();

  const Params params(m_settings->getParams(pageId));
  m_pageId = pageId;
  m_colorParams = params.colorParams();
  m_outputProcessingParams = m_settings->getOutputProcessingParams(pageId);

  updateView();
}

void SauvolaModBinarizationOptionsWidget::windowSizeChanged(int value) {
  BlackWhiteOptions opt(m_colorParams.blackWhiteOptions());
  opt.setWindowSize(value);
  m_colorParams.setBlackWhiteOptions(opt);
  m_settings->setColorParams(m_pageId, m_colorParams);

  m_delayedStateChanger.start(750);
}

void SauvolaModBinarizationOptionsWidget::sauvolaCoefChanged(double value) {
  BlackWhiteOptions opt(m_colorParams.blackWhiteOptions());
  opt.setSauvolaCoef(value);
  m_colorParams.setBlackWhiteOptions(opt);
  m_settings->setColorParams(m_pageId, m_colorParams);

  m_delayedStateChanger.start(750);
}

void SauvolaModBinarizationOptionsWidget::updateView() {
  BlackWhiteOptions blackWhiteOptions = m_colorParams.blackWhiteOptions();
  windowSize->setValue(blackWhiteOptions.getWindowSize());
  sauvolaCoef->setValue(blackWhiteOptions.getSauvolaCoef());
}

void SauvolaModBinarizationOptionsWidget::sendStateChanged() {
  emit stateChanged();
}

#define CONNECT(...) m_connectionManager.addConnection(connect(__VA_ARGS__))

void SauvolaModBinarizationOptionsWidget::setupUiConnections() {
  CONNECT(windowSize, SIGNAL(valueChanged(int)), this, SLOT(windowSizeChanged(int)));
  CONNECT(sauvolaCoef, SIGNAL(valueChanged(double)), this, SLOT(sauvolaCoefChanged(double)));
  CONNECT(&m_delayedStateChanger, SIGNAL(timeout()), this, SLOT(sendStateChanged()));
}

#undef CONNECT
}  // namespace output
