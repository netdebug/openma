/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solution Inc., all rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __openma_base_nodeid_h
#define __openma_base_nodeid_h

#include "openma/base/typeid.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

/**
 * Define the public method isCastable() used to determine if the object can be cast to the given type.
 * @note This macro must be included by every inheriting Node classes to be correctly recognised as suche. For example, the function node_cast() needs the use of this macro to correctly work.
 * @relates ma::Node
 * @ingroup openma_base
 */
#define OPENMA_DECLARE_NODEID(derivedclass,baseclass) \
  public: \
  static_assert(!std::is_same<baseclass,derivedclass>::value,"The base class cannot be the same than the current class."); \
  virtual bool isCastable(ma::typeid_t id) const _OPENMA_NOEXCEPT override \
  { \
    if (this->baseclass::isCastable(id)) \
      return true; \
    return (ma::static_typeid<derivedclass>() == id); \
  }; \
  private:

#endif // __openma_base_nodeid_h