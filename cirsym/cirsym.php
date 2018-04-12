<html>
 <head>
  <title>PHP</title>
 </head>
 <body>
<?php
//Этот скрипт реализует запуск заранее скомпилированной под linux программы cirsym, предварительно узнав у пользователя исходные данные (файл)
//и данные конфигурации, которые из формы пишутся в setup.sum. После успешной загрузки файла на сервер и выполнения программы, данные выкидываются пользователю
//в виде файла input_file_name.out. Если исходный файл корявый - прога напишет об этом в выходном файле и пользователь получит уведомление.
//После отработки все временные файлы подтираются с сервера. НЕ реализована многопоточность

//Проверяем размер файла
if($_FILES["userfile"]["size"] > 1024*1024)
   {
     exit("Размер файла превышает мегабайт");
   }
else {
echo ("Размер нормальный");
}
$upload_dir = '/home/starkov/intersyn.net/cirsym/';

  // Проверяем загружен и перемещен ли файл
   if(is_uploaded_file($_FILES["userfile"]["tmp_name"]))
   {
       $tmp_name = $_FILES["userfile"]["tmp_name"];
       $name = basename($_FILES["userfile"]["name"]);
       $output = $name.'.out';
       echo(", загружен");
       if(move_uploaded_file($tmp_name , "$upload_dir/$name"))
       {
           echo (", перемещен");
       } else {
           exit(", невозможно переместить файл");
       }

       $file_content = file_get_contents($name);
	//if (strpos($file_content, '.END') || strpos($file_content, '.end') && strpos($file_content, '.total') || strpos($file_content, '.TOTAL')) {
       //if ( strpos(strtolower($file_content), '.end') && strpos(strtolower($file_content), '.total')) {
       //    echo(", проверка на .END и/или .total пройдена.");
       //} else {
       //    exit(", в файле отсутствует .END и/или .total");
       //}
	if (!strpos(strtolower($file_content), '.total')) {
           $file_content .= "\n".'.total';
           file_put_contents($name, $file_content);
        }
   } else {
       exit(", не удалось загрузить файл");
   }
 //программа видит только файлы с названием 1...99, причем может обработать за раз несколько
   if(rename("$upload_dir/$name","$upload_dir"."/96"))
{
echo (", переименован.");
}
else
{
 exit(", не удалось переименовать файл.");
}

//Загружаем инфу из формы и сразу пишем в файл
$fp = fopen("setup.sym", "w");
fwrite($fp, "Parameters of 5-bisection:\n");
fwrite($fp, $_POST['5bi']);
fwrite($fp, "\nParameters of 4-bisection:\n");
fwrite($fp, $_POST['4bi']);
fwrite($fp, "\nParameters of 3-bisection:\n");
fwrite($fp, $_POST['3bi']);
fwrite($fp, "\nParameters of 2-bisection:\n");
fwrite($fp, $_POST['2bi']);
fwrite($fp, "\nOrder of extraction (passive - 0, active - 1)\n");
fwrite($fp, $_POST['order']);
fwrite($fp, "\nSingle bisection method? (1 - yes, 0 - no)\n");
fwrite($fp, $_POST['bi_method']);
fwrite($fp, "\nOutput 's' (1) or 'p' (0)\n");
fwrite($fp, $_POST['output']);
fwrite($fp, "\nComplement equivalent transformation? (0 - no, 1 - yes)\n");
fwrite($fp, $_POST['transform']);
fwrite($fp, "\nOutput 's' (or 'p') to the formula (1 - yes, 0 - no)\n");
fwrite($fp, $_POST['formula']);
fwrite($fp, "\nPolinomial network function (1 - yes, 0 - no)\n");
fwrite($fp, $_POST['polinom']);
fwrite($fp, "\nNullor equvalent (1 - yes, 0 - no)\n");
fwrite($fp, $_POST['nullor']);
fwrite($fp, "\nDublicates (1 - yes, 0 - no)\n");
fwrite($fp, $_POST['Dublicates']);
fwrite($fp, "\nOdd loop (1 - yes, 0 - no)\n");
fwrite($fp, $_POST['loop']);

fclose($fp);
// Запускаем функцию и переименовываем инпут для пользователя
if(shell_exec("./cirsym")==NULL)
         {
             unlink("96");
             exit("  Не удается запустить Cirsym");
         }
//Output check
if (strpos(file_get_contents("96.out"), "Cannot execute")){
        unlink("96");
        unlink("96.out");
        echo ("<script>alert('Файл с исходными данными был сформирован неверно. Отредактируйте файл и попробуйте снова.');</script>");
        header('Refresh: 0; URL=http://intersyn.net/cirsym.html');
        exit;
}
else {
       unlink("96");
       print_output($_POST['onlyformuls']);
       rename("96.out",$output);
       echo ("\n  Output renamed");
}


// //Отдаем файл пользователю
//file_force_download($output);
unlink($output);

function print_output($formula) {
    $output = str_replace(' ', '', file_get_contents("96.out"));
    if ($formula) {
        $lower_output = strtolower($output);
        $result = [];

        if ($start = strripos($lower_output, 'detan=')) {
            $stop = strpos($lower_output, ";", $start) - $start;
        } elseif (preg_match_all("/[u]\d=/", $lower_output, $result) ||
            preg_match_all("/[i]\d=/", $lower_output, $result)) {
            $token = $result[0][0];
            $start = strrpos($lower_output, $token);
            $stop = strpos($lower_output, ";", $start) - $start;
        }
        print_r('<br>'.'<h3>'.substr($output, $start, $stop).'</h3>');
    } else {
        print_r('<br>'.str_replace('%', '<br>', $output));
    }
}

function print_output_old($formula) {
    $output = str_replace(' ', '', file_get_contents("96.out"));
    print_r($formula);
    if ($formula) {
        $lower_output = strtolower($output);
        $result = [];
        
        if ($start = strripos($lower_output, 'detan=')) {
            $stop = strpos($lower_output, ";", $start) - $start;
        } elseif (preg_match_all("/[u]\d=/", $lower_output, $result) ||
            preg_match_all("/[i]\d=/", $lower_output, $result)) {
            $token = $result[0][0];
            $start = strpos($lower_output, $token);
            $stop = strpos($lower_output, ";", $start) - $start;
        }

        print_r('<br>'.'<h3>'.substr($output, $start, $stop).'</h3>');
    } else {
        print_r('<br>'.str_replace('%', '<br>', $output));
    }
}

function file_force_download($file) {
  if (file_exists($file)) {
    // сбрасываем буфер вывода PHP, чтобы избежать переполнения памяти выделенной под скрипт
    // если этого не сделать файл будет читаться в память полностью!
    if (ob_get_level()) {
      ob_end_clean();
    }
    // заставляем браузер показать окно сохранения файла
    header('Content-Description: File Transfer');
    header('Content-Type: application/octet-stream');
    header('Content-Disposition: attachment; filename=' . basename($file));
    header('Content-Transfer-Encoding: binary');
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($file));
    // читаем файл и отправляем его пользователю
    readfile($file);
  }
}

exit;
?>
 </body>
</html>
