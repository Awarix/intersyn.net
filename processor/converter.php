<?php

$input = $_POST["input"];
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
foreach ($lines as $line => $line_val) {
	echo $line_val. "<br>";
}
