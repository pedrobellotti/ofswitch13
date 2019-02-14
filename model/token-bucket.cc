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

#include "token-bucket.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TokenBucket");
NS_OBJECT_ENSURE_REGISTERED (TokenBucket);

TokenBucket::TokenBucket ()
  : m_lastRefill (Simulator::Now ()),
  m_tokens (0)
{
  NS_LOG_FUNCTION (this);
}

TypeId
TokenBucket::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TokenBucket")
    .SetParent<Object> ()
    .SetGroupName ("OFSwitch13")
    .AddConstructor<TokenBucket> ()
    .AddAttribute ("TimeoutInterval",
                   "The interval between each token refill.",
                   TimeValue (MilliSeconds (100)),
                   MakeTimeAccessor (&TokenBucket::m_timeout),
                   MakeTimeChecker (MilliSeconds (1), MilliSeconds (1000)))
    .AddAttribute ("Size",
                   "The size of the bucket, in bits.",
                   UintegerValue (0),
                   MakeUintegerAccessor (&TokenBucket::m_size),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Rate",
                   "The average long-run token rate.",
                   DataRateValue (DataRate ("100Gb/s")),
                   MakeDataRateAccessor (&TokenBucket::m_rate),
                   MakeDataRateChecker ())
  ;
  return tid;
}

void
TokenBucket::Timeout ()
{
  NS_LOG_FUNCTION (this);
  Time elapsed = Simulator::Now () - m_lastRefill;
  uint32_t addTokens = (m_rate.GetBitRate () * elapsed.GetMilliSeconds ()) / 1000;
  m_tokens = std::min (m_tokens + addTokens, m_size);
  m_lastRefill = Simulator::Now ();
  Simulator::Schedule (m_timeout, &TokenBucket::Timeout, this);
}

bool
TokenBucket::removeTokens (uint32_t numTokens)
{
  NS_LOG_FUNCTION (this << numTokens);
  if (m_tokens >= numTokens)
    {
      m_tokens -= numTokens;
      return true;
    }
  else
    {
      return false;
    }
}

/*Função para testes - será removida*/
void
TokenBucket::imprime ()
{
  NS_LOG_FUNCTION (this);
  std::cout << "m_size: " << m_size << std::endl;
  std::cout << "m_timeout: " << m_timeout << std::endl;
  std::cout << "m_lastRefill: " << m_lastRefill << std::endl;
  std::cout << "m_rate: " << m_rate << std::endl;
  std::cout << "m_tokens: " << m_tokens << std::endl;
}

TokenBucket::~TokenBucket ()
{
  NS_LOG_FUNCTION (this);
}

/********** Protected methods **********/
void
TokenBucket::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  Object::DoDispose ();
}

void
TokenBucket::NotifyConstructionCompleted ()
{
  NS_LOG_FUNCTION (this);

  // Execute the first timeout.
  Timeout ();

  // Chain up.
  Object::NotifyConstructionCompleted ();
}

} // namespace ns3
