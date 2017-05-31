#include "stdafx.h"
#include "vtokenizer.h"
#include "text_stream.h"
#include "compile_error.h"

vds::vtokenizer::vtokenizer(
  vds::text_stream& s,
  const char ** operators)
: tokenizer<vtoken>(s), operators_(operators)
{
  if (!s.is_eof()) {
    this->next();
  }
}

const vds::vtoken& vds::vtokenizer::next()
{
  if (this->is_eof()) {
    this->set_current(TT_EOF);
    return this->current();
  }

  for(;;) {
   if(
     '_' == this->next_symbol().symbol
     || isalpha(this->next_symbol().symbol)){
     
     std::string & value = this->set_current(TT_WORD);
     
     while(
       '_' == this->next_symbol().symbol
       || isalnum(this->next_symbol().symbol)){
       value += this->next_symbol().symbol;
       if(!this->read_next()){
         break;
       }
     }
     
     break;
   }
   else if(isdigit(this->next_symbol().symbol)) {
     std::string & value = this->set_current(TT_NUMBER);
     
     while(isdigit(this->next_symbol().symbol)){
       value += this->next_symbol().symbol;
       if(!this->read_next()){
         break;
       }
     }
     
     break;
   }
   else if(isspace(this->next_symbol().symbol)) {
     while(isspace(this->next_symbol().symbol)){
       if(!this->read_next()){
        this->set_current(TT_EOF);
        return this->current();
       }
     }
     
     continue;
   }
   else if(this->next_symbol().symbol == '\"') {
     std::string & value = this->set_current(TT_STRING);

     for (;;) {
      if (!this->read_next()) {
        this->compile_error("Expected \"");
      }

      while ('\"' != this->next_symbol().symbol) {
        value += this->next_symbol().symbol;
        if (!this->read_next()) {
          this->compile_error("Expected \"");
        }
      }

      if (!this->read_next() || '\"' != this->next_symbol().symbol) {
        break;
      }

      //"" -> empty
      //"123""456" -> 123"456
      //"""ww" -> "w
      value += this->next_symbol().symbol;
     }
     break;
   }
   else {
     std::string & value = this->set_current(TT_OPERATOR);
     
     for(;;) {
      std::string newValue = value;
      newValue += this->next_symbol().symbol;
      bool bGood = false;
      for(const char ** op = this->operators_; nullptr != *op; ++op){
        if(*op == newValue) {
          bGood = true;
          break;
        }
      }
      
      if(bGood) {
        value = newValue;
        if(!this->read_next()){
          break;
        }
      }
      else {
        break;
      }
     }
     
     if(value.empty()) {
       this->compile_error(std::string("Invalid token ") + this->next_symbol().symbol);
     }
     
     //Inline comment
     if(value == "//") {
       while(this->next_symbol().symbol != '\n') {
        if(!this->read_next()){
          break;
        }
       }
       
       continue;
     }
     
     //Multi line comment
     if(value == "/*") {
       for(;;) {
        while('*' != this->next_symbol().symbol) {
          if(!this->read_next()){
            this->compile_error(
              "Unexpected end of file while looking for end of comment");
          }
        }
          
        if(!this->read_next()){
          this->compile_error(
            "Unexpected end of file while looking for end of comment");
        }
          
        if('/' == this->next_symbol().symbol) {
          break;
        }
      }
      
      if(!this->read_next()){
        this->set_current(TT_EOF);
        return this->current();
      }
      continue;
     }
     
     break;
   }
  }
  
  return this->current();
}
