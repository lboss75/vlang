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
	}

	class tokenizer
	{
	}
}