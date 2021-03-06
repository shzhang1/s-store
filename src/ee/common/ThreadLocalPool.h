/* This file is part of VoltDB.
 * Copyright (C) 2008-2011 VoltDB Inc.
 *
 * VoltDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VoltDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VoltDB.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Copyright (C) 2017 by S-Store Project
 * Brown University
 * Massachusetts Institute of Technology
 * Portland State University 
 *
 * Author: S-Store Team (sstore.cs.brown.edu)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef THREADLOCALPOOL_H_
#define THREADLOCALPOOL_H_

#include "CompactingStringStorage.h"

#include "boost/pool/pool.hpp"
#include "boost/shared_ptr.hpp"

namespace voltdb {

struct voltdb_pool_allocator_new_delete
{
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  static char * malloc(const size_type bytes);
  static void free(char * const block);
};

/**
 * A wrapper around a set of pools that are local to the current thread.
 * An instance of the thread local pool must be maintained somewhere in the thread to ensure initialization
 * and destruction of the thread local pools. Creating multiple instances is fine, it is reference counted. The thread local
 * instance of pools will be freed once the last ThreadLocalPool reference in the thread is destructed.
 */
class ThreadLocalPool {
public:
    ThreadLocalPool();
    ~ThreadLocalPool();

    static void initialize();

    /**
     * Retrieve a pool that allocates approximately sized chunks of memory. Provides pools that
     * are powers of two and powers of two + the previous power of two.
     */
    static boost::shared_ptr<boost::pool<voltdb_pool_allocator_new_delete> > get(std::size_t size);

    /**
     * Retrieve a pool that allocate chunks that are exactly the requested size. Only creates
     * pools up to 1 megabyte + 4 bytes.
     */
    static boost::shared_ptr<boost::pool<voltdb_pool_allocator_new_delete> > getExact(std::size_t size);

    static std::size_t getPoolAllocationSize();

    static CompactingStringStorage* getStringPool();

};
}

#endif /* THREADLOCALPOOL_H_ */
