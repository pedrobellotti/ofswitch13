/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 University of Campinas (Unicamp)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Luciano Chaves <luciano@lrc.ic.unicamp.br>
 */

#include "traffic-policing.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TrafficPolicing");
NS_OBJECT_ENSURE_REGISTERED (TrafficPolicing);

TrafficPolicing::TrafficPolicing ()
  : m_bucketSize (0),
  m_refillTime (Seconds(0)),
  m_lastRefill (Simulator::Now ()),
  m_expectedThroughput (0),
  m_tokens (0),
  m_consumedTokens (0)
{
  ScheduleRefill();
}

TypeId
TrafficPolicing::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TrafficPolicing")
    .SetParent<Object> ()
    .SetGroupName ("OFSwitch13")
    .AddConstructor<TrafficPolicing> ()
    .AddAttribute ("RefillInterval",
                   "The interval between each token refill.",
                   TypeId::ATTR_SGC,
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&TrafficPolicing::m_refillTime),
                   MakeTimeChecker (MilliSeconds (1), MilliSeconds (1000)))
    .AddAttribute ("BucketSize",
                   "Size of the bucket",
                   TypeId::ATTR_SGC,
                   UintegerValue (0),
                   MakeUintegerAccessor (&TrafficPolicing::m_bucketSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("ExpectedThroughput",
                   "Expected throughput",
                   TypeId::ATTR_SGC,
                   DataRateValue (DataRate ("10Mbps")),
                   MakeDataRateAccessor (&TrafficPolicing::m_expectedThroughput),
                   MakeDataRateChecker ())
  ;
  return tid;
}

TypeId
TrafficPolicing::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
TrafficPolicing::ScheduleRefill ()
{
  uint64_t addTokens = (m_expectedThroughput.GetBitRate() * m_refillTime.GetMilliSeconds()) / 1000;
  uint64_t maxTokens = m_expectedThroughput.GetBitRate ();
  m_tokens = std::min (m_tokens + addTokens, maxTokens);
  m_lastRefill = Simulator::Now ();
  Simulator::Schedule (m_refillTime, &TrafficPolicing::ScheduleRefill, this);
}

void
TrafficPolicing::removeTokens (uint32_t tokens)
{
  m_tokens -= tokens;
  m_consumedTokens += tokens;
}

bool
TrafficPolicing::checkTokens (uint32_t pktSize)
{
  if (m_tokens >= pktSize)
  {
    return true;
  }
  else
  {
    return false;
  }  
}

void
TrafficPolicing::imprime(){
  std::cout << "m_bucketSize: " << m_bucketSize << std::endl;
  std::cout << "m_refillTime: " << m_refillTime << std::endl;
  std::cout << "m_lastRefill: " << m_lastRefill << std::endl;
  std::cout << "m_expectedThroughput: " << m_expectedThroughput << std::endl;
  std::cout << "m_tokens: " << m_tokens << std::endl;
  std::cout << "m_consumedTokens: " << m_consumedTokens << std::endl;

}

TrafficPolicing::~TrafficPolicing ()
{
  NS_LOG_FUNCTION (this);
}
/********** Protected methods **********/
void
TrafficPolicing::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  Object::DoDispose ();
}

} // namespace ns3
