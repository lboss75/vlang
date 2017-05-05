#ifndef __TEXT_STREAM_H_
#define __TEXT_STREAM_H_

#include <string>

#include "text_symbol.h"

namespace vds {
    class text_stream
    {
    public:
      text_stream(const std::string & filename);
        ~text_stream();
        
        text_symbol read();
        bool is_eof();

        const std::string & filename() const
        {
          return this->filename_;
        }

    private:
        std::string filename_;
        int handle_;

        char buffer_[4096];
        size_t position_;
        size_t size_;
        bool is_eof_;
        int line_;
        int column_;
    };
};

#endif//__TEXT_STREAM_H_
