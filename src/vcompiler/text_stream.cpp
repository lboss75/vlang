#include "stdafx.h"
#include "text_stream.h"

vds::text_stream::text_stream(const std::string & filename)
: size_(0), position_(0), is_eof_(false),
line_(1), column_(1), filename_(filename)
{
#ifndef _WIN32
  this->handle_ = open(filename.c_str(), O_RDONLY);
  if (0 > this->handle_) {
    auto error = errno;
    throw std::system_error(error, std::generic_category(), "Unable to open file " + filename);
  }
#else
    this->handle_ = _open(filename.c_str(), _O_RDONLY);
    if (0 > this->handle_) {
      auto error = GetLastError();
      throw std::system_error(error, std::system_category(), "Unable to open file " + filename);
    }
#endif
    
}
  
vds::text_stream::~text_stream()
{
#ifndef _WIN32
    close(this->handle_);
#else
    _close(this->handle_);
#endif
}

bool vds::text_stream::is_eof()
{
  if (this->position_ < this->size_) {
    return false;
  }
  
  if (!this->is_eof_) {
    this->size_ = ::read(
      this->handle_,
      this->buffer_,
      sizeof(this->buffer_));
    this->position_ = 0;     
    if(this->size_ == 0) {
      this->is_eof_ = true;
    }
  }
  return this->is_eof_;
}

vds::text_symbol vds::text_stream::read()
{
  if (this->is_eof()) {
    throw new std::logic_error("End of file");
  }

  auto line = this->line_;
  auto column = this->column_;

  auto result = this->buffer_[this->position_++];
  if (result == '\n') {
      this->line_++;
      this->column_ = 1;
  }
  else if (result == '\t') {
    this->column_ = 1 + ((this->column_ + 7) & ~7);
  }
  else {
      this->column_++;
  }

  return text_symbol { result, line, column };
}
