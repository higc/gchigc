<?php
/*
	핵심 파일 로드 및 기본 설정
*/
include_once "configs.php";
include_once "db.lib.php";

include_once "secu.lib.php";

session_start();
header("Content-Type: text/html; charset=".$melon['charset']);
//error_reporting(E_ALL);
ini_set("display_errors", 1);
error_reporting(E_ERROR | E_WARNING | E_PARSE);

/*
	매개변수 정리
*/
if( !get_magic_quotes_gpc() )
{
	$_REQUEST = sqlInjectionClean($_REQUEST);
	$_COOKIE = sqlInjectionClean($_COOKIE);
}
unset($_GET);
unset($_POST);
unset($_REQUEST["PHPSESSID"]);
$param = &$_REQUEST;
$session = &$_SESSION;

xssClean($param);
/*
	데이터베이스 연결
*/
if($melon['db']['id']&&$melon['db']['pw']&&$melon['db']['name']&&$melon['db']['host']) {
	dbConnect();
}
set_error_handler("handleError");
if($melon['helper']['uri']){
	parseURI();
}

function handleError($errno, $errstr, $errfile, $errline, array $errcontext){
	if(strpos($errstr,"mysql")!==false){
		throw new Exception($errstr);
	}
	else{
		return false;
	}
}
/*
if( !function_exists( '_log' ) )
{
	function _log( $data )
	{
		echo '<script>';
		echo 'console.log( '.json_encode( $data ).' );';
		echo '</script>';
	}
_log( 'Parameters' );
_log( $param );
_log( 'SERVER Variables' );
_log( $_SERVER );
_log( 'sessions' );
_log( $_SESSION );
}
*/
/**
 * 
 * URI를 SEGMENT기반의 URI로 변경함.
 */
function parseURI(){
	GLOBAL $param;
	GLOBAL $melon;
	GLOBAL $session;
	$URI = array();
	$URI_EXPLODE = explode('?', $_SERVER['REQUEST_URI']);
	if (is_array($URI_EXPLODE) === TRUE && count($URI_EXPLODE) > 1) {
		$URI['url'] = $URI_EXPLODE[0];
		$URI['params'] = $URI_EXPLODE[1];

	} else {

		$URI['url'] = $URI_EXPLODE[0];
		$URI['params'] = '';

	}
	$URI['path_info'] = $_SERVER['PATH_INFO'];
	$URI['self'] = $_SERVER['PHP_SELF'];
	$SEGMENT = explode('/', $URI['url']);
	while( !empty($SEGMENT)&&in_array($SEGMENT[0], array('', 'index')) === TRUE ) {
		array_shift($SEGMENT);
	}
	$melon['segment']=$SEGMENT;
	if(empty($SEGMENT)){
		$includePath='controllers/index.php';
	}
	else{

		$dir='controllers';
	
		while(!empty($SEGMENT)&&is_dir($dir.'/'.$SEGMENT[0])){
		
			if(!$SEGMENT[0]){
				break;
			}
			$dir.='/'.$SEGMENT[0];
			array_shift($SEGMENT);
		}
		
		$melon['dir']=str_replace('controllers','',$dir);
		$melon['parent']=array_pop(explode('/',$dir));
		if(is_file($dir.'/'.$SEGMENT[0].'.php')){
			$includePath=$dir.'/'.$SEGMENT[0].'.php';
			$melon['self']=$SEGMENT[0];
			array_shift($SEGMENT);
		}
		else{
			$includePath=$dir.'/index.php';
			$melon['self']='index';
		}
	}
	$melon['path'] = str_replace('/index','',$melon['dir'].'/'.$melon['self']);


	if($melon['singleParam'][$melon['dir'].'/'.$melon['self']]){
		$singleParam=$melon['singleParam'][$melon['dir'].'/'.$melon['self']]; //  /admin/main/test/index/ 전체경로+파일명이 index
		
	}	
	if($melon['singleParam'][$melon['dir']]){

		$singleParam=$melon['singleParam'][$melon['dir']];  //		/admin/main/test/       전체경로가같은 하위페이지

	}	
	if($melon['singleParam'][$melon['parent'].'/'.$melon['self']]){
		$singleParam=$melon['singleParam'][$melon['parent'].'/'.$melon['self']]; //		main/index       main밑에 index인 페이지

	}
	if($melon['singleParam'][$melon['parent'].'/']){
		$singleParam=$melon['singleParam'][$melon['parent'].'/']; //	main/    : 부모디렉토리가 main인 페이지

	}
	if($melon['singleParam'][$melon['self']]){
		$singleParam=$melon['singleParam'][$melon['self']]; // ex) index      파일명이 index

	}

	if($singleParam){
		$cnt = 0;
		foreach($singleParam as $key=>$value){
			$shift=$key-$cnt;
			$result=array_splice($SEGMENT,$shift,1);
			$param[$value]=urldecode($result[0]);
			$cnt++;
		}
	}
	$len=count($SEGMENT);

	for($iu=0;$iu<=$len;$iu++){
		if($iu%2==1){
			if(!in_array($SEGMENT[$iu-1],$melon['param'])&&$SEGMENT[$iu-1]&&!is_numeric($SEGMENT[$iu-1])){
				
				include 'errors/error_404.html';
				exit;
			}
			$param[$SEGMENT[$iu-1]]=urldecode($SEGMENT[$iu]);
		}
	}
	if(is_file($includePath)||!is_numeric($SEGMENT[$iu-1])){
		include'models/common.php';
		if($melon['segment'][0]=='admin'){
			include'models/admin.php';
			$melon['is_admin_page'] = true;
			
		}
		else{
			include'models/user.php';
			$melon['is_admin'] = false;
		}
		if(!is_file($includePath)){
			include 'errors/error_404.html';
			exit;
		}
		
		include $includePath;
	}
	else{
		include 'errors/error_404.html';
		exit;
	}
}
?>


