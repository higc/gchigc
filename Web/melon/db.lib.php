<?php

/**
 * 데이터베이스에 접속
 * @param mixed $db 데이터베이스 정보를 담은 객체
 */
function dbConnect($db=null)
{
	GLOBAL $melon;
	if($db==null){$db = &$melon['db'];}else if(is_string($db)){$db = $melon[$db];}
	else if(is_string($db)){$db = &$melon[$db];}
	$db['connect'] = mysqli_connect($db['host'], $db['id'], $db['pw'], $db['name']);
	if (!$db['connect'] ) {
	   echo mysqli_connect_error();
	   return false;
	}
	mysqli_set_charset($db['connect'], str_replace("-","",$melon['charset']));
}

/**
 * 데이터베이스 접속해제
 * @param mixed $db 데이터베이스 정보를 담은 객체
 */
function dbDisconnect($db=null)
{
	GLOBAL $melon;
	if($db==null){$db = &$melon['db'];}
	
	if($db['connect']){mysqli_close($db['connect']);}
	unset($db['connect']);
	unset($db['select']);

}



/**
 * 지정한 조건의 목록을 가져온다. 현재 가져온 컬럼의 수를 length로 결과배열에 포함해서 반환한다.
 * @param String $table_name 테이블명
 * @param String $where 검색 조건
 * @param String $fields 가져올 데이터 필드 지정
 * @param String $order 정렬 순서
 * @param Number $start 시작번호
 * @param Number $len 길이
 * @param mixed $db 데이터베이스 정보를 담은 객체
 * @return mixed 결과
 */
function getList($table, $where="",  $start=null,$len="", $order="", $fields="*", $db=null, $viewQuery=false)
{
	GLOBAL $melon;
	if(strpos($table,',viewQuery')){
		$table=str_replace(',viewQuery','',$table);
		$viewQuery=true;
	}
	if($db==null){$db = &$melon['db'];}else if(is_string($db)){$db = $melon[$db];}
	if(!$db['connect']){
		echo '<strong>MySql Error</strong>: database is not connected.';
		return false;
	}
	$i=0;
	$query = "SELECT ".getFieldSet($fields)." FROM ".$table;
	$query .= getWhere($where);
	if(trim($order)!=""){$query .= " ORDER BY ".$order;}
	else{if(!empty($melon["column"]["index"])){$order = $melon['column']['index']." DESC";$query .= " ORDER BY ".$order;}else{$order="";}}
	if(isset($start)&&$start!==''){
		if($len==''){
			$query .= " LIMIT ".$start;
		}
		else{
			$query .= " LIMIT ".$start.",".$len;
		}
	}
	if($viewQuery){echo $query;}
	try{
		$result = mysqli_query($db['connect'], $query);
		
		$row['list'] = array();
		while($data = mysqli_fetch_assoc($result))
		{
			$row['list'][$i] = $data;
			$i++;
		}
		$row['length'] = $i;
	}catch (Exception $e){
		$row = array();
		$mysqlError=  mysqli_error($db['connect']);
		if($mysqlError) {
			echo '<strong>MySql Error</strong>: '.$mysqlError.'<br><strong>Query : </strong>'.$query.'<br>';

		}
	}
	
	return $row;
}



/**
 * 입력한 문자열로 필드에 대한 쿼리 문자열을 자동생성한다.
 * @param String $param 필드 정의
 * @return String 결과 쿼리
 */
function getFieldSet(&$param)
{
	if(trim($param)==""){return "*";}
	else if(is_string($param)){return $param;}
	else if(is_array($param)){
		$result = "";
		foreach($param as $key=>$value)
		{
			if(!is_numeric($key))
			{
				if($result!=""){$result.=",";}
				if(trim($value)==""){
					$result.=$key;
				}else{
					$result.=$value." as ".$key;
				}
			}
		}
		
		return $result;
	}
	else{return "*";}
}

/**
 * 입력한 문자열로 조건문에 대한 쿼리 문자열을 자동생성한다.
 * @param String $where 검색 조건
 * @return String 결과 쿼리
 */
function getWhere(&$where)
{
	GLOBAL $melon;
	
	$result = "";
	if(trim($where)==""){}
	else if(is_numeric($where)){$result .= " WHERE ".$melon['column']['index']."='".$where."'";}
	else if(!preg_match('/[^0-9,]/i',$where)){$result .= " WHERE ".$melon['column']['index']." in (".$where.")";}
	else if(!preg_match('/[^0-9a-zA-Z가-핳,]/i',$where))
	{$result .= " WHERE ".$melon['column']['index']." in ('".preg_replace("/,/","','",$where)."')";}
	else{$result .= " WHERE ".$where;}
	
	return $result;
}

/**
 * 입력한 문자열로 필드에 대한 쿼리 문자열을 자동생성한다.
 * @param String $table_name 테이블명
 * @param String $param 필드 정의
 * @return String 결과 쿼리
 */
function getSet($table, &$param, $db=null)
{
	GLOBAL $melon;
	if($db==null){$db = &$melon['db'];}else if(is_string($db)){$db = $melon[$db];}
	else if(is_string($db)){$db = &$melon[$db];}
	
	$columns = getTableColumns($table, $db);
	$set = "";
	$j = 0;
	foreach($param as $key=>$value)
	{
		if(gettype($value)=='NULL'){
			continue;
		}
		if(array_key_exists($key, $columns) && $columns[$key]['name']!="" && !is_numeric($key))
		{
			
			if($j!=0){$set .= ",";}
			if($value=='now()'){
				$set .= $key."=".$value."";
			}
			else if(strpos($value,'PASSWORD(')!==FALSE){
			$set .= $key."=".$value."";
			}
			else{
				$set .= $key."='".$value."'";
			}
			$j++;
		}
	}
	
	return $set;
}



function getTableColumns($table, $db=null,$viewQuery=false)
{
	GLOBAL $melon;
	if($db==null){$db = &$melon['db'];}else if(is_string($db)){$db = $melon[$db];}
	else if(is_string($db)){$db = &$melon[$db];}
	
	if(isset($melon['table'][$table]))
	{
		$row = $melon['table'][$table];
	}
	else
	{
		$i = 0;

		$query = "SHOW COLUMNS FROM ".$table;
		
		if($viewQuery){echo $query;}
		try{
			$result = mysqli_query($db['connect'], $query);
			while($data = mysqli_fetch_assoc($result))
			{
				$row[$data['Field']]['name'] = $data['Field'];
				$i++;
			}
		}catch (Exception $e){
			$row = array();
			$mysqlError=  mysqli_error($db['connect']);
			if($mysqlError) {
				echo '<strong>MySql Error</strong>: '.$mysqlError.'<br><strong>Query : </strong>'.$query.'<br>';

			}
		}

		
		$melon['table'][$table] = $row;
	}
	return $row;
}
?>