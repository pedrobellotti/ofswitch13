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

#ifndef TRAFFIC_POLICING_H
#define TRAFFIC_POLICING_H

#include <ns3/core-module.h>
#include <ns3/data-rate.h>


namespace ns3 {

/**
 * \ingroup ofswitch13
 * Traffic policing using token bucket algorithm
 */
class TrafficPolicing : public Object
{
public:
  /**
   * Register this type.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  TrafficPolicing ();            //!< Default constructor
  virtual ~TrafficPolicing ();   //!< Dummy destructor, see DoDispose

  /**
   * Complete constructor.
   * \param bucketSize Size of the bucket.
   */
  TrafficPolicing (uint32_t bucketSize);


  /** \return The queue id */
  uint32_t GetQueueId (void) const;

  /**
   * Removes tokens from the bucket.
   * \param tokens The amount of tokens to be removed
   * \return True if there were enough tokens, False if not
   */
  bool removeTokens (uint32_t pktSize);

  /* TESTE */
  void imprime ();

protected:
  // Inherited from Object
  virtual void DoDispose (void);

private:
  uint32_t m_bucketSize;  //!< Bucket size (burst size)
  Time m_refillTime;  //!< Time between each token refill
  Time m_lastRefill;  //!< Time since last refill
  DataRate m_rate;  //!< Expected throughput
  uint32_t m_tokens;  //!< Amount of tokens stored
  uint32_t m_consumedTokens;  //!< Total amount of tokens consumed
  /**
   * Schedules the next refill.
   */
  void Timeout ();
};

} // namespace ns3
#endif // TRAFFIC_POLICING_H

