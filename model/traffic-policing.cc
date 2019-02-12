/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2019 Federal University of Juiz de Fora (UFJF)
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
 * Author: Pedro Bellotti <pedro.bellotti@ice.ufjf.br>
 * Author: João Victor Guimarães <joaoguimaraes@ice.ufjf.br>
 */

#include "traffic-policing.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TrafficPolicing");
NS_OBJECT_ENSURE_REGISTERED (TrafficPolicing);

TrafficPolicing::TrafficPolicing ()
  : m_bucketSize (0),
  m_refillTime (Seconds (0)),
  m_lastRefill (Simulator::Now ()),
  m_rate (0),
  m_tokens (0),
  m_consumedTokens (0)
{
  Timeout ();
}

TypeId
TrafficPolicing::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TrafficPolicing")
    .SetParent<Object> ()
    .SetGroupName ("OFSwitch13")
    .AddConstructor<TrafficPolicing> ()
    .AddAttribute ("TimeoutInterval",
                   "The interval between each token refill.",
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&TrafficPolicing::m_refillTime),
                   MakeTimeChecker (MilliSeconds (1), MilliSeconds (1000)))
    .AddAttribute ("BucketSize",
                   "Size of the bucket in bits",
                   UintegerValue (0),
                   MakeUintegerAccessor (&TrafficPolicing::m_bucketSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Rate",
                   "Expected throughput (rate)",
                   DataRateValue (DataRate ("10Mbps")),
                   MakeDataRateAccessor (&TrafficPolicing::m_rate),
                   MakeDataRateChecker ())
  ;
  return tid;
}

void
TrafficPolicing::Timeout ()
{
  NS_LOG_FUNCTION (this);
  Time elapsed = Simulator::Now () - m_lastRefill;
  uint32_t addTokens = (m_rate.GetBitRate () * elapsed.GetMilliSeconds ()) / 1000;
  std::cout << "add " << addTokens << std::endl;
  m_tokens = std::min (m_tokens + addTokens, m_bucketSize);
  std::cout << "tokens " << m_tokens << std::endl;
  m_lastRefill = Simulator::Now ();
  Simulator::Schedule (m_refillTime, &TrafficPolicing::Timeout, this);
}

bool
TrafficPolicing::removeTokens (uint32_t pktSize)
{
  NS_LOG_FUNCTION (this);
  if (m_tokens >= pktSize)
    {
      m_tokens -= pktSize;
      m_consumedTokens += pktSize;
      return true;
    }
  else
    {
      return false;
    }
}

/*Função para testes - será removida*/
void
TrafficPolicing::imprime ()
{
  NS_LOG_FUNCTION (this);
  std::cout << "m_bucketSize: " << m_bucketSize << std::endl;
  std::cout << "m_refillTime: " << m_refillTime << std::endl;
  std::cout << "m_lastRefill: " << m_lastRefill << std::endl;
  std::cout << "m_rate: " << m_rate << std::endl;
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
