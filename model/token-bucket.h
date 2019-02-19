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

#ifndef TOKEN_BUCKET_H
#define TOKEN_BUCKET_H

#include <ns3/core-module.h>
#include <ns3/data-rate.h>


namespace ns3 {

/**
 * \ingroup ofswitch13
 * Token bucket algorithm
 */
class TokenBucket : public Object
{
public:
  /**
   * Register this type.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  TokenBucket ();            //!< Default constructor
  virtual ~TokenBucket ();   //!< Dummy destructor, see DoDispose

  /**
   * Removes tokens from the bucket.
   * \param tokens The amount of tokens to be removed
   * \return True if there were enough tokens, False if not
   */
  bool removeTokens (uint64_t numTokens);

protected:
  // Inherited from Object
  virtual void DoDispose (void);

  // Inherited from ObjectBase.
  virtual void NotifyConstructionCompleted (void);

private:
  /**
   * Schedules the next refill.
   */
  void Timeout ();

  uint64_t    m_size;            //!< Bucket size (burst size)
  Time        m_timeout;         //!< Time between each token refill
  Time        m_lastRefill;      //!< Time since last refill
  DataRate    m_rate;            //!< Expected throughput
  uint64_t    m_tokens;          //!< Amount of tokens stored

};

} // namespace ns3
#endif // TOKEN_BUCKET_H

