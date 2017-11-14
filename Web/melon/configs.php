<?php	
	$melon['debug']						="DEBUG";
	$melon['charset']					= 'utf-8';		//Default Encoding
	$melon['column']['index']			= '';		//Sequence field name
	$melon['column']['create']		= 'create_date';	//Create date field name
	$melon['column']['update']		= 'modify_date';		//Update date field name
	$melon['upload']['filter'] = "php|htm|html";
	$melon['db']['type']				= 'mysqli';		//Database Type
	$melon['db']['host']			= 'localhost';	//Database Host
	$melon['db']['id']				= 'root';	//Database Connection ID
	$melon['db']['pw']				= '';	//Database Connection Password
	$melon['db']['name']			= 'guardicycle';	//Database Name
	$melon['helper']['pagination'] = array(
		'first'=>'<a href="[url]">&lt;&lt;</a>',
		'prev'=>'<a href="[url]">&lt;</a>',
		'number'=>'<a href="[url]">$page</a>',
		'next'=>'<a href="[url]">&gt;</a>',
		'last'=>'<a href="[url]">&gt;&gt;</a>',
		'current'=>'<a href="[url]" class="active">$page</a>'
	);


	/*
		Segment URI Setting
	*/
	
	$melon['helper']['uri']			 = false;
//	$melon['singleParam']['/boardtest/boardtest2/index']=array(0=>'board_id',1=>'board_mode'); //Mono Parameter
//	$melon['singleParam']['/test']=array(0=>'board_id',1=>'board_mode'); //Mono Parameter
	//$melon['singleParam']['index']=array(0=>'test'); //Mono Parameter
	$melon['singleParam']['board/']=array(0=>'board_id',1=>'board_mode'); //Mono Parameter
	$melon['param']=array('page','no','search_type','search_keyword','board_name','board_mode','mode');  //valid parameter

//db 에러는 500에러로 돌리는것 개선
?>