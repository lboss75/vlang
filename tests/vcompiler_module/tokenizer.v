package vcompiler
{
  "version": "0.0.1"
}

namespace v {

	class text_symbol
	{
	  public property line : Integer;
	  public property column : Integer;
	  public property symbol : Char;
	}

	class filename
	{
		public filename(name: String)
		{
			this.value = name;
		}

		property value : String;
	}

	class text_stream
	{
		public external text_stream(filename: String);
		public external read() : text_symbol?;
		public property	current : text_symbol?;
	}

	class token
	{
	}
        
	class word_token : token
	{
		word_token(
			line : Integer,
	  		column : Integer,
			value : String)
		{
			this.line = line;
			this.column = column;
			this.value = value;
		}

	  public property line : Integer;
	  public property column : Integer;
	  public property value : String;
        }


	class tokenizer
	{
		public tokenizer(stream : text_stream) {
			this.stream_ = stream;
		}

		public next_token() : token?
		{
			var ch = this.stream_.current;
			if(!ch) {
				return null;
			}

			for(;;) {
        if("_" == ch.symbol || ch.symbol.isalpha){
          var first_char = ch;
     
          var value : String;
          while("_" == ch.symbol || ch.symbol.isalnum){
				       value += ch.symbol;
				       ch = this.stream_.read();
          }

          return new word_token(
            line: first_char.line,
            column: first_char.column,
            value: value);
        }
      }
    }
    
    property stream_ : text_stream;
	}

	class vcompiler
	{
		public static compile(file_to_compile: filename)
		{
			var text_stream = new text_stream(filename: file_to_compile);
			var t = new tokenizer(stream: text_stream);
			var l = new vlexer(tokenizer: t);
			var t = new vsyntax(lexer: l);

			var tree = t.parse();

			cpp_generator.generate(tree: tree);
		}
	}
}