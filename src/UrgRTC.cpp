// -*- C++ -*-
/*!
 * @file  UrgRTC.cpp
 * @brief Hokuyo URG RTC
 * @date $Date$
 *
 * $Id$
 */

#include "UrgRTC.h"

// Module specification
// <rtc-template block="module_spec">
static const char* urgrtc_spec[] =
  {
    "implementation_id", "UrgRTC",
    "type_name",         "UrgRTC",
    "description",       "Hokuyo URG RTC",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Senso",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.port_name", "COM1",
    "conf.default.baudrate", "115200",
    "conf.default.debug", "0",
    "conf.default.encoding", "2",
    // Widget
    "conf.__widget__.port_name", "text",
    "conf.__widget__.baudrate", "text",
    "conf.__widget__.debug", "text",
    "conf.__widget__.encoding", "spin",
    // Constraints
    "conf.__constraints__.encoding", "{twochar: 2, threechar: 3}",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
UrgRTC::UrgRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rangeOut("range", m_range)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
UrgRTC::~UrgRTC()
{
}



RTC::ReturnCode_t UrgRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("range", m_rangeOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("port_name", m_port_name, "COM1");
  bindParameter("baudrate", m_baudrate, "115200");
  bindParameter("debug", m_debug, "0");
  bindParameter("encoding", m_encoding, "2");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t UrgRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t UrgRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t UrgRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t UrgRTC::onActivated(RTC::UniqueId ec_id)
{
  m_pUrg = new ssr::UrgBase(m_port_name.c_str(), m_baudrate);
  m_pUrg->startMeasure();

  coil::usleep(1000);

  ssr::RangeData r = m_pUrg->getRangeData();
  m_range.config.minAngle = r.minAngle;
  m_range.config.maxAngle = r.maxAngle;
  m_range.config.angularRes = r.angularRes;
  m_range.config.minRange = r.minRange;
  m_range.config.maxRange = r.maxRange;
  //m_range.config.rangeRes = r.rangeRes;
  //m_range.config.frequency = r.frequency
  return RTC::RTC_OK;
}


RTC::ReturnCode_t UrgRTC::onDeactivated(RTC::UniqueId ec_id)
{
  m_pUrg->reset();
  delete m_pUrg;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t UrgRTC::onExecute(RTC::UniqueId ec_id)
{
  ssr::RangeData r = m_pUrg->getRangeData();
  m_range.config.minAngle = r.minAngle;
  m_range.config.maxAngle = r.maxAngle;
  if (r.length != m_range.ranges.length()) {
    m_range.ranges.length(r.length);
  }

  for(int i = 0;i < r.length;i++) {
    m_range.ranges[i] = r.range[i];
  }
  m_rangeOut.write();

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t UrgRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t UrgRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t UrgRTC::onReset(RTC::UniqueId ec_id)
{
  if (m_pUrg) {
    m_pUrg->reset();
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t UrgRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t UrgRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void UrgRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(urgrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<UrgRTC>,
                             RTC::Delete<UrgRTC>);
  }
  
};


