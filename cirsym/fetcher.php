<?php 

class Core{

	private $response;
	private $raw_file;
	private $file_urn  = NULL;
	private $initial_data;
	private $numeric = "69";
	private $canonical_path = '/home/starkov/intersyn.net/cirsym/';

	function __construct($action){

		if ($action == 'feed'){

			$this->numeric = $this->canonical_path.$this->numeric;
			$this->raw_file = json_encode($_FILES["file_contents"]);
			$this->initial_data = json_encode($_POST);
			$this->UploadRawFile();

			if (!$this->IsLock()){

				$this->Lock();

				if($this->Initialize()){

					if($this->Validate()){

						if($this->Exec()){

							$this->Relocate();
							$this->response["status"] = "OK";

						}else{
							$this->response["error_code"] = "Runtime error";
						}
					}else{
							$this->response["error_code"] = "Invalid File";
					}
				}else{
					$this->response["error_code"] = "No file specified";
				}

				$this->Unlock();

			}else{
				$this->response["error_code"] = "Cirsym is being executed by another application";
				$this->Clean();
			}

			$this->response["urn"] = $this->file_urn;
			$this->Clean();

			

		}

	}

	function __destruct(){

		if (!empty($this->response["error_code"])){
			$this->response["status"] = "Error";
			$this->response["urn"] = NULL;
		}

		header('Content-type: application/json');
		echo json_encode($this->response);
	}

	private function UploadRawFile(){

		$file = json_decode($this->raw_file);
		
		if($file->size > 0 ){

			if ($file->size > 1024){
			
				$this->response["error_code"] = "File is too big ".$file->size;
				return 0;
			}

			$target_dir = $this->canonical_path."uploads/";
			$urn = md5(uniqid($file->name, true));
			$target_file = $target_dir . basename($urn);
			if (move_uploaded_file($file->tmp_name, $target_file)) {

		        $this->file_urn = $target_file;
		        return true;

		    } else {
				$this->response["error_code"] = "Error occured while uploading!";
				return false;
		    }

		}else{
			$this->response["error_code"] = "No file specified";
			return false;
		}
	}

	private function Lock(){
		$fp = fopen("lock", "w");
		fclose($fp);
	}

	private function Unlock(){
		if(file_exists("lock")){
			unlink("lock");
		}
	}

	private function IsLock(){
		$arr = scandir(realpath(dirname(__FILE__)));
		foreach ($arr as $key => $value) {
			if ($value == "lock"){
				$lock = true;
				break;
			}
			else{
				continue;
			}
		}
		if(isset($lock)){
			return true;
		}else{
			return false;
		}
	}

	private function Initialize($data = array()){

		if($this->initial_data){
			$data = json_decode($this->initial_data, 1);
		}
		
			$fp = fopen("setup.sym", "w");
			fwrite($fp, "Parameters of 5-bisection:\n");
			fwrite($fp, $data['5bi']);
			fwrite($fp, "\nParameters of 4-bisection:\n");
			fwrite($fp, $data['4bi']);
			fwrite($fp, "\nParameters of 3-bisection:\n");
			fwrite($fp, $data['3bi']);
			fwrite($fp, "\nParameters of 2-bisection:\n");
			fwrite($fp, $data['2bi']);
			fwrite($fp, "\nOrder of extraction (passive - 0, active - 1)\n");
			fwrite($fp, $data['order']);
			fwrite($fp, "\nSingle bisection method? (1 - yes, 0 - no)\n");
			fwrite($fp, $data['bi_method']);
			fwrite($fp, "\nOutput 's' (1) or 'p' (0)\n");
			fwrite($fp, $data['output']);
			fwrite($fp, "\nComplement equivalent transformation? (0 - no, 1 - yes)\n");
			fwrite($fp, $data['transform']);
			fwrite($fp, "\nOutput 's' (or 'p') to the formula (1 - yes, 0 - no)\n");
			fwrite($fp, $data['formula']);
			fwrite($fp, "\nPolinomial network function (1 - yes, 0 - no)\n");
			fwrite($fp, $data['polinom']);
			fwrite($fp, "\nNullor equvalent (1 - yes, 0 - no)\n");
			fwrite($fp, $data['nullor']);
			fwrite($fp, "\nDublicates (1 - yes, 0 - no)\n");
			fwrite($fp, $data['Dublicates']);
			fwrite($fp, "\nOdd loop (1 - yes, 0 - no)\n");
			fwrite($fp, $data['loop']);
			fclose($fp);

			if(file_exists($this->file_urn)){
				copy($this->file_urn, $this->numeric);
				return true;
			}else{
				return false;
			}
	}

	private function Validate(){

		$handle = file($this->file_urn);
		if ($handle) {
			$last_line = end($handle);
			if(strpos(strtolower($last_line), '.total') !== false){
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}

	private function Exec(){

		if(shell_exec($this->canonical_path."cirsym") != NULL)
        {
        	return true;
        }else{
        	return false;
        }

	}

	private function Relocate(){
		copy($this->numeric.".out", $this->file_urn.".out");
	}

	private function Clean(){

		if (file_exists($this->numeric)){
			unlink($this->numeric);
		}
		if (file_exists($this->file_urn)){
			unlink($this->file_urn);
		}
		if (file_exists($this->numeric.".out")){
			unlink($this->numeric.".out");
		}
		if (file_exists("setup.sym")){
			unlink("setup.sym");
		}
	}
}

$inst = new Core($_GET['action']);
?>
