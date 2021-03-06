/**
 * @file io/cxx/HDF5Exception.cc
 * @date Wed Jun 22 17:50:08 2011 +0200
 * @author Andre Anjos <andre.anjos@idiap.ch>
 *
 * @brief Implements exceptions that can be thrown by the HDF5 support code.
 *
 * Copyright (C) 2011-2013 Idiap Research Institute, Martigny, Switzerland
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/format.hpp>
#include <bob/io/HDF5Exception.h>
#include <bob/io/HDF5Types.h>

bob::io::HDF5Exception::HDF5Exception() throw()
{
}

bob::io::HDF5Exception::~HDF5Exception() throw() { }

const char* bob::io::HDF5Exception::what() const throw() {
  static const char* emergency = "Generic HDF5 exception. You should never see this!";
  return emergency;
}

bob::io::HDF5UnsupportedCxxTypeError::HDF5UnsupportedCxxTypeError() throw():
  bob::io::HDF5Exception()
{
}

bob::io::HDF5UnsupportedCxxTypeError::~HDF5UnsupportedCxxTypeError() throw() { }

const char* bob::io::HDF5UnsupportedCxxTypeError::what() const throw() {
  try {
    boost::format message("The C++ type given is not supported by our HDF5 interface");
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5UnsupportedTypeError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5UnsupportedTypeError::HDF5UnsupportedTypeError(const boost::shared_ptr<hid_t>&) throw():
  bob::io::HDF5Exception()
{
}

bob::io::HDF5UnsupportedTypeError::HDF5UnsupportedTypeError() throw():
  bob::io::HDF5Exception()
{
}

bob::io::HDF5UnsupportedTypeError::~HDF5UnsupportedTypeError() throw() { }

const char* bob::io::HDF5UnsupportedTypeError::what() const throw() {
  try {
    boost::format message("The C++ type given is not supported by our HDF5 interface");
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5UnsupportedTypeError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5UnsupportedDimensionError::HDF5UnsupportedDimensionError(size_t n_dim) 
  throw():
  bob::io::HDF5Exception(), 
  m_n_dim(n_dim)
{
}

bob::io::HDF5UnsupportedDimensionError::~HDF5UnsupportedDimensionError() throw() { }

const char* bob::io::HDF5UnsupportedDimensionError::what() const throw() {
  try {
    boost::format message("Got an array with %u dimensions, but we only support up to %d dimensions");
    message % m_n_dim % bob::core::array::N_MAX_DIMENSIONS_ARRAY;
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5UnsupportedDimensionError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5InvalidFileAccessModeError::HDF5InvalidFileAccessModeError
(const unsigned int mode) throw():
  bob::io::HDF5Exception(),
  m_mode(mode)
{
}

bob::io::HDF5InvalidFileAccessModeError::~HDF5InvalidFileAccessModeError() throw() { }

const char* bob::io::HDF5InvalidFileAccessModeError::what() const throw() {
  try {
    boost::format message("Trying to use an undefined access mode '%d'");
    message % m_mode;
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5InvalidFileAccessModeError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5StatusError::HDF5StatusError(const std::string& call, herr_t status)
  throw():
    bob::io::HDF5Exception(),
    m_call(call),
    m_status(status)
{
}

bob::io::HDF5StatusError::~HDF5StatusError() throw() { }

std::string bob::io::format_hdf5_error() {
  const std::vector<std::string>& stack = bob::io::DefaultHDF5ErrorStack->get();
  std::ostringstream retval;
  std::string prefix(" ");
  if (stack.size()) retval << prefix << stack[0];
  for (size_t i=1; i<stack.size(); ++i) 
    retval << std::endl << prefix << stack[i];
  bob::io::DefaultHDF5ErrorStack->clear();
  return retval.str();
}

const char* bob::io::HDF5StatusError::what() const throw() {
  try {
    boost::format message("Call to HDF5 C-function '%s' returned '%d'. HDF5 error statck follows:\n%s");
    message % m_call % m_status % format_hdf5_error();
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5StatusError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5IndexError::HDF5IndexError(const std::string& location,
    size_t size, size_t asked_for) throw():
  bob::io::HDF5Exception(),
  m_location(location),
  m_size(size),
  m_asked_for(asked_for)
{
}

bob::io::HDF5IndexError::~HDF5IndexError() throw() { }

const char* bob::io::HDF5IndexError::what() const throw() {
  try {
    boost::format message("Trying to access element %d in Dataset '%s' that only contains %d elements");
    message % m_asked_for % m_location % m_size;
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5IndexError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5IncompatibleIO::HDF5IncompatibleIO(const std::string& location, 
    const std::string& supported,
    const std::string& user_input) throw():
  bob::io::HDF5Exception(),
  m_location(location),
  m_supported(supported),
  m_user_input(user_input)
{
}

bob::io::HDF5IncompatibleIO::~HDF5IncompatibleIO() throw() { }

const char* bob::io::HDF5IncompatibleIO::what() const throw() {
  try {
    boost::format message("Trying to read or write '%s' at '%s' that only accepts '%s'");
    message % m_user_input % m_location % m_supported;
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5IndexError: cannot format, exception raised";
    return emergency;
  }
}

bob::io::HDF5NotExpandible::HDF5NotExpandible(const std::string& location) throw():
  bob::io::HDF5Exception(),
  m_location(location)
{
}

bob::io::HDF5NotExpandible::~HDF5NotExpandible() throw() { }

const char* bob::io::HDF5NotExpandible::what() const throw() {
  try {
    boost::format message("Trying to append to '%s' that is not expandible");
    message % m_location;
    m_message = message.str();
    return m_message.c_str();
  } catch (...) {
    static const char* emergency = "bob::io::HDF5NotExpandible: cannot format, exception raised";
    return emergency;
  }
}
