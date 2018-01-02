/* Copyright (c) 2016, 2018, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
  */

#ifndef TABLE_ESMH_GLOBAL_H
#define TABLE_ESMH_GLOBAL_H

/**
  @file storage/perfschema/table_esmh_global.h
  Table EVENTS_STATEMENTS_HISTOGRAM_GLOBAL (declarations).
*/

#include "storage/perfschema/table_helper.h"

/**
  @addtogroup performance_schema_tables
  @{
*/

class PFS_index_esmh_global : public PFS_engine_index
{
public:
  PFS_index_esmh_global() : PFS_engine_index(&m_key_1), m_key_1("BUCKET_NUMBER")
  {
  }

  ~PFS_index_esmh_global()
  {
  }

  bool match_bucket(ulong bucket_index);

private:
  PFS_key_bucket_number m_key_1;
};

/**
  A row of table
  PERFORMANCE_SCHEMA.EVENTS_STATEMENTS_HISTOGRAM_GLOBAL.
*/

struct PFS_esmh_global_bucket
{
  /** Column BUCKET_NUMBER. */
  ulonglong m_count_bucket;
  /** Column COUNT_BUCKET_AND_LOWER. */
  ulonglong m_count_bucket_and_lower;
};

struct PFS_esmh_global_histogram
{
  /** Statistics for all buckets. */
  PFS_esmh_global_bucket m_buckets[NUMBER_OF_BUCKETS];
};

struct row_esmh_global
{
  /** Column BUCKET_NUMBER. */
  ulong m_bucket_number;
  /** Column BUCKET_TIMER_LOW. */
  ulonglong m_bucket_timer_low;
  /** Column BUCKET_TIMER_HIGH. */
  ulonglong m_bucket_timer_high;
  /** Column COUNT_BUCKET. */
  ulonglong m_count_bucket;
  /** Column COUNT_BUCKET_AND_LOWER. */
  ulonglong m_count_bucket_and_lower;
  /** Column BUCKET_QUANTILE. */
  double m_percentile;
};

/** Table PERFORMANCE_SCHEMA.EVENTS_STATEMENTS_HISTOGRAM_GLOBAL. */
class table_esmh_global : public PFS_engine_table
{
  typedef PFS_simple_index pos_t;

public:
  /** Table share */
  static PFS_engine_table_share m_share;
  static PFS_engine_table *create(PFS_engine_table_share *);
  static int delete_all_rows();
  static ha_rows get_row_count();

  virtual void reset_position(void);

  virtual int rnd_next();
  virtual int rnd_pos(const void *pos);

  virtual int index_init(uint idx, bool sorted);
  virtual int index_next();

protected:
  virtual int read_row_values(TABLE *table,
                              unsigned char *buf,
                              Field **fields,
                              bool read_all);

  table_esmh_global();

public:
  ~table_esmh_global()
  {
  }

protected:
  void materialize();
  int make_row(ulong bucket_index);

private:
  /** Table share lock. */
  static THR_LOCK m_table_lock;
  /** Table definition. */
  static Plugin_table m_table_def;

  /** Current row. */
  PFS_esmh_global_histogram m_materialized_histogram;
  row_esmh_global m_row;
  /** Current position. */
  pos_t m_pos;
  /** Next position. */
  pos_t m_next_pos;

  PFS_index_esmh_global *m_opened_index;

  bool m_materialized;
};

/** @} */
#endif
