<?php

class Core{

	private $response;
	private $raw_file;
	private $file_urn  = NULL;
	private $initial_data;
	private $numeric = "69";
	private $canonical_path = '/home/starkov/intersyn.net/api/';
	private $domain = "http://intersyn.net/api/";

	function __construct($action){

		if ($action == 'feed'){

			$this->numeric = $this->canonical_path.$this->numeric;
			$this->raw_file = json_encode($_FILES["file_contents"]);
			$this->initial_data = json_encode($_POST);
			if ($this->UploadRawFile()){
				if (!$this->IsLock()){

					$this->Lock();

					if($this->Initialize()){

						if($this->Validate()){

							if(!$this->Double_entry()){

								if($this->Exec()){

									$this->Relocate();
									$this->response["status"] = "OK";

								}else{
									$this->response["error_code"] = "Runtime error";
								}
							}else{
									$this->response["error_code"] = "Only one input source and only one response are allowed in a cir-file";
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
			}
			$this->response["urn"] = str_replace($this->canonical_path, $this->domain, $this->file_urn);
			$this->Clean();



		}

	}

	function __destruct(){

		if (!empty($this->response["error_code"])){
			$this->response["status"] = "Error";
			$this->response["urn"] = "null";
		}

		header('Access-Control-Allow-Origin: *');
		header('Content-type: application/json');
		echo json_encode($this->response);
	}

	private function UploadRawFile(){

		$file = json_decode($this->raw_file);

		if($file->size > 0 ){

			if ($file->size > 1024 * 1024){

				$this->response["error_code"] = "File is too big ".$file->size;
				return false;
			}

			$target_dir = $this->canonical_path."uploads/";
			$urn = md5(uniqid($file->name, true));
			$target_file = $target_dir . basename($urn);
			if (move_uploaded_file($file->tmp_name, $target_file)) {

		        $this->file_urn = $target_file;
		        // Convert
		        $data = json_decode($this->initial_data, 1);
		        if ($data["isLTFormat"] === "true") {
		        	$buffer = file_get_contents($target_file); 
		        	$buffer = explode("\n", $buffer);
		        	$processed = $this->Convert(file_get_contents($target_file));
		        	$new_file = fopen($target_file, "w");
					foreach ($processed as $key => $value) {
						fwrite($new_file, $value."\r\n");
					}
					fclose($new_file);
		        }
		        return true;

		    } else {
				$this->response["error_code"] = "Error occured while uploading";
				return false;
		    }

		}else{
			$this->response["error_code"] = "No file specified";
			return false;
		}
	}

	private function Convert($input){

		$lines = explode("\n", $input);

		$ou_voltage_inputs = array();

		$iut_voltages = array();
		$iut_vals = array();

		$iun_voltages = array();
		$iun_vals = array();

		foreach ($lines as $line => $line_val) {

			if($line >= 0) {
				$ou_buff = array();
				$iut_buff = array();
				$iun_buff = array();
				// N001 => 1
				$new_val = preg_replace("/([N])(0+)(\d)/", "$3", $line_val);
				// E{d} => K{d} 
				$new_val = preg_replace("/([E])(\d+)/", "K$2", $new_val);
				// V{d} x y => V{d} y x
				$new_val = preg_replace("/([V])(\d+)(\s)(\d)(\s)(\d)/", "$1$2$3$6$5$4", $new_val);
				// V{d} => E{d}
				$new_val = preg_replace("/([V])(\d+)/", "E$2", $new_val);
				// I{d} => J{d} 
				$new_val = preg_replace("/([I])(\d+)/", "J$2", $new_val);
				// AC || SINE || PULSE || EXP (...) => 1
				$new_val = preg_replace("/(AC)(.)*/", "1", $new_val);
				$new_val = preg_replace("/(SINE)(.)*/", "1", $new_val);
				$new_val = preg_replace("/(PULSE)(.)*/", "1", $new_val);
				$new_val = preg_replace("/(EXP)(.)*/", "1", $new_val);
				// r{d} => R{d}
				$new_val = preg_replace("/[r](\d+)/", "R$1", $new_val);
				// C{d} => c{d}
				$new_val = preg_replace("/[C](\d+)/", "c$1", $new_val);
				// l{d} => L{d}
				$new_val = preg_replace("/[l](\d+)/", "L$1", $new_val);
				// R{d} {d} {d} {d} tol=10 pwr=0.1.... => R{d} {d} {d} {d}
				$new_val = preg_replace("/([R]\d+\s\d+\s\d+\s\d+)(.)*/", "$1", $new_val);
				// c{d} {d} {d} {d} tol=10 pwr=0.1.... => c{d} {d} {d} {d}
				$new_val = preg_replace("/([c]\d+\s\d+\s\d+\s\d+)(.)*/", "$1", $new_val);
				// L{d} {d} {d} {d} tol=10 pwr=0.1.... => L{d} {d} {d} {d}
				$new_val = preg_replace("/([L]\d+\s\d+\s\d+\s\d+)(.)*/", "$1", $new_val);

				// Search for ou
				preg_match("/(XU)(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)(.)*/", $new_val, $ou_buff);
				if (!empty($ou_buff)){
					//we found ou, then we need to extract voltage inputs
					$v1 = $ou_buff[5];
					$v2 = $ou_buff[6];

					array_push($ou_voltage_inputs, $v1);
					array_push($ou_voltage_inputs, $v2);
				}

				// XU{d} {d} {d} {d} {d} {d} LT6220... => N{d} {d} {d} 0 {d}
				$new_val = preg_replace("/(XU)(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)(.)*/", "N$2 $3 $4 0 $7", $new_val);

				// process iut
				preg_match("/([F])(\d+)\s(\d+)\s(\d+)\s(E)(\d+)(.)*/", $new_val, $iut_buff);
				if(!empty($iut_buff)) {
					array_push($iut_voltages, $iut_buff[6]);
					$fill_val = preg_replace("/([F])(\d+)\s(\d+)\s(\d+)\s(E)(\d+)\s(\d+)*/", "$1$2 $3 $4 $7", $new_val);
					array_push($iut_vals, $fill_val);

					unset($lines[$line]);
					continue;
				}

				// process iun
				preg_match("/([H])(\d+)\s(\d+)\s(\d+)\s(E)(\d+)(.)*/", $new_val, $iun_buff);
				if(!empty($iun_buff)) {
					array_push($iun_voltages, $iun_buff[6]);
					$fill_val = preg_replace("/([H])(\d+)\s(\d+)\s(\d+)\s(E)(\d+)\s(\d+)*/", "$1$2 $3 $4 $7", $new_val);
					array_push($iun_vals, $fill_val);

					unset($lines[$line]);
					continue;
				}
			} else {
				$new_val = $line_val;
			}

			$lines[$line] = $new_val;
		}

		// delete all voltage inputs for ou in the scheme
		if(!empty($ou_voltage_inputs)) {
			foreach ($lines as $line => $line_val) {
				$node = preg_replace("/([E])(\d+)\s(\d)+\s(\d)+(.)*/", "$4", $line_val);
				foreach ($ou_voltage_inputs as $input) {
					if ($node == $input) {
						unset($lines[$line]);
					}
				}
			}
		}

		// replace all voltages for iut in the scheme
		if(!empty($iut_voltages)) {
			foreach ($lines as $line => $line_val) {
				$node = array();
				preg_match("/\A([E])(\d+)(.)*/", $line_val, $node);
				if (!empty($node)) {
					foreach ($iut_voltages as $key => $input) {
						if ($node[2] == $input) {
							$to_fill = preg_replace("/([E])(\d+)\s(\d+)\s(\d+).*/", "$3 $4", $line_val);
							$lines[$line] = preg_replace("/([F])(\d+)\s(\d+)\s(\d+)\s(\d+)/", "B$2 ".$to_fill." $3 $4 $5", $iut_vals[$key]);
						}
					}
				}
			}
		}

		// replace all voltages for iut in the scheme
		if(!empty($iun_voltages)) {
			foreach ($lines as $line => $line_val) {
				$node = array();
				preg_match("/\A([E])(\d+)(.)*/", $line_val, $node);
				if (!empty($node)) {
					foreach ($iun_voltages as $key => $input) {
						if ($node[2] == $input) {
							$to_fill = preg_replace("/([E])(\d+)\s(\d+)\s(\d+).*/", "$3 $4", $line_val);
							$lines[$line] = preg_replace("/([H])(\d+)\s(\d+)\s(\d+)\s(\d+)/", "H$2 ".$to_fill." $3 $4 $5", $iun_vals[$key]);
						}
					}
				}
			}
		}

		array_push($lines, ".total");

		return $lines;
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
			fwrite($fp, $data['poli_func']);
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

		$data = json_decode($this->initial_data, 1);
		if ($data["isLTFormat"] == true) {
			return true;
		}

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

	private function Double_entry(){

		$handle = file($this->file_urn);

		$e = 0;
		$i = 0;
		$j = 0;
		$u = 0;

		foreach ($handle as $key => $value) {

			if(preg_match('/([E][0-9])+/', $value)){
				$e++;
			}
			if(preg_match('/([I][0-9])+/', $value)){
				$i++;
			}

			if(preg_match('/([J][0-9])+/', $value)){
				$j++;
			}

			if(preg_match('/([U][0-9])+/', $value)){
				$u++;
			}
		}

		if(($e > 1) || ($i > 1) || ($j > 1) || ($u > 1) || (($u = 1) && ($i = 1))){
			return false;
		}else{
			return true;
		}
	}

	private function Exec(){
		if(shell_exec($this->canonical_path."cirsym_v2.1.bin") != NULL)
        {
        	return true;
        }else{
        	return false;
        }

	}

	private function Relocate(){
		var_dump(copy($this->numeric.".out", $this->file_urn.".out"));
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

