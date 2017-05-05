namespace v {
	class vcompiler {

		argument_set default {
			argument() foldername[] modules;
		}

		public static main() : void {
			for(var m in this.modules){
				compile_module(m);
			}
		}
		
		compile(input:file) : void {
			for(;;){
				var token = input.get_token();
				if(!token){
					break;
				}
			}
		}
		
	}
}