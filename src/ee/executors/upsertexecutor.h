/* This file is part of VoltDB.
* Copyright (C) 2008-2010 VoltDB L.L.C.
*
* This file contains original code and/or modifications of original code.
* Any modifications made by VoltDB L.L.C. are licensed under the following
* terms and conditions:
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
/* Copyright (C) 2008 by H-Store Project
* Brown University
* Massachusetts Institute of Technology
* Yale University
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

#ifndef HSTOREUPSERTEXECUTOR_H
#define HSTOREUPSERTEXECUTOR_H

#include "common/common.h"
#include "common/valuevector.h"
#include "common/tabletuple.h"
#include "executors/abstractexecutor.h"
#include "boost/shared_array.hpp"

namespace voltdb {

	class UpsertPlanNode;
	class Table;

	/**
	*
	*/
	class UpsertExecutor : public OperationExecutor {
	public:
		UpsertExecutor(VoltDBEngine *engine, AbstractPlanNode* abstract_node) : OperationExecutor(engine, abstract_node) {
			m_inputTable = NULL;
			m_targetTable = NULL;
			m_node = NULL;
			m_engine = engine;
			m_partitionColumn = -1;
			m_multiPartition = false;
                        m_inputTargetMapSize = -1;
		}

		~UpsertExecutor();

	protected:
		bool p_init(AbstractPlanNode*, const catalog::Database* catalog_db, int* tempTableMemoryInBytes);
		bool p_execute(const NValueArray &params, ReadWriteTracker *tracker);

		virtual bool needsOutputTableClear() { return true; };

		UpsertPlanNode* m_node;

		Table* m_inputTable;
		Table* m_targetTable;

		TableTuple m_tuple;
		int m_partitionColumn;
		bool m_partitionColumnIsString;
		bool m_multiPartition;

		/** reference to the engine/context to store the number of modified tuples */
		VoltDBEngine* m_engine;

		// added by hawk, 10/4/2013
		std::vector<std::pair<int, int> > m_inputTargetMap;
		int m_inputTargetMapSize;

		TableTuple m_targetTuple; 
		TableIndex *m_primarykey_index;
		TableTuple m_searchKey;
		char* m_searchKeyBackingStore;
		int m_primarykey_size;
		int* m_primarykeysArray;
		boost::shared_array<int> m_primarykeysArrayPtr;
		// ended by hawk
	};

}

#endif
