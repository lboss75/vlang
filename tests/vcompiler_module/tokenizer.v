package vcompiler
{
  "version": "0.0.1"
}

namespace v {
	class text_symbol
	{
	  public property line : int;
	  public property column : int;
	  public property symbol : char;
	}

	class text_stream
	{
		public external text_stream(filename: string);
		public external read() : text_symbol?;
		public property	current : text_symbol?;
	}

	class token
	{
	}
        
	class word_token : token
	{
		word_token(
			line : int,
	  		column : int,
			value : string)
		{
		}

	  public property line : int;
	  public property column : int;
	  public property value : string;
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
			   if('_' == ch.symbol || ch.symbol.isalpha){
				var first_char = ch;
     
     				string value;
				while('_' == ch.symbol || ch.symbol.isalnum){
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
	
		private property stream_ : text_stream;
	}
}