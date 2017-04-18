namespace v {
	application vcompiler {

		argument_set default {
			argument() foldername[] modules;
		}

		void main(){
			for(var m in this.modules){
				compile_module(m);
			}
		}
		
		void 
		
	}
}