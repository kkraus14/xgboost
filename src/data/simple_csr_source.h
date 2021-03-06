/*!
 * Copyright 2015 by Contributors
 * \file simple_csr_source.h
 * \brief The simplest form of data source, can be used to create DMatrix.
 *  This is an in-memory data structure that holds the data in row oriented format.
 * \author Tianqi Chen
 */
#ifndef XGBOOST_DATA_SIMPLE_CSR_SOURCE_H_
#define XGBOOST_DATA_SIMPLE_CSR_SOURCE_H_

#include <xgboost/base.h>
#include <xgboost/data.h>

#include <algorithm>
#include <string>
#include <vector>

#include "columnar.h"

namespace xgboost {
namespace data {
/*!
 * \brief The simplest form of data holder, can be used to create DMatrix.
 *  This is an in-memory data structure that holds the data in row oriented format.
 * \code
 * std::unique_ptr<DataSource> source(new SimpleCSRSource());
 * // add data to source
 * DMatrix* dmat = DMatrix::Create(std::move(source));
 * \encode
 */
class SimpleCSRSource : public DataSource<SparsePage> {
 public:
  // MetaInfo info;  // inheritated from DataSource
  SparsePage page_;
  /*! \brief default constructor */
  SimpleCSRSource() = default;
  /*! \brief destructor */
  ~SimpleCSRSource() override = default;
  /*! \brief clear the data structure */
  void Clear();
  /*!
   * \brief copy content of data from src
   * \param src source data iter.
   */
  void CopyFrom(DMatrix* src);
  /*!
   * \brief copy content of data from parser, also set the additional information.
   * \param src source data iter.
   * \param info The additional information reflected in the parser.
   */
  void CopyFrom(dmlc::Parser<uint32_t>* src);
  /*!
   * \brief copy content of data from foreign **GPU** columnar buffer.
   * \param interfaces_str JSON representation of cuda array interfaces.
   */
  void CopyFrom(std::string const& cuda_interfaces_str);
  /*!
   * \brief Load data from binary stream.
   * \param fi the pointer to load data from.
   */
  void LoadBinary(dmlc::Stream* fi);
  /*!
   * \brief Save data into binary stream
   * \param fo The output stream.
   */
  void SaveBinary(dmlc::Stream* fo) const;
  // implement Next
  bool Next() override;
  // implement BeforeFirst
  void BeforeFirst() override;
  // implement Value
  const SparsePage &Value() const override;
  /*! \brief magic number used to identify SimpleCSRSource */
  static const int kMagic = 0xffffab01;

 private:
  /*!
   * \brief copy content of data from foreign GPU columnar buffer.
   * \param cols foreign columns data buffer.
   */
  void FromDeviceColumnar(std::vector<Columnar> cols);
  /*! \brief internal variable, used to support iterator interface */
  bool at_first_{true};
};
}  // namespace data
}  // namespace xgboost
#endif  // XGBOOST_DATA_SIMPLE_CSR_SOURCE_H_
