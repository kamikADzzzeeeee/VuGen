Action()
{

	lr_start_transaction("UC6_RegistrationNewUser");

	lr_start_transaction("GoTo_LoginPage");

	web_set_sockets_option("SSL_VERSION", "2&3");

	web_add_header("Sec-Fetch-Dest", 
		"document");

	web_add_header("Sec-Fetch-Site", 
		"none");

	web_add_auto_header("Sec-Fetch-Mode", 
		"navigate");

	web_add_header("Sec-Fetch-User", 
		"?1");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	web_url("WebTours", 
		"URL=http://localhost:1080/WebTours/", 
		"Resource=0", 
		"Referer=", 
		"Snapshot=t1.inf", 
		"Mode=HTML", 
		LAST);

	web_add_auto_header("Sec-Fetch-Dest", 
		"frame");

	web_add_auto_header("Sec-Fetch-Site", 
		"same-origin");

	web_url("header.html", 
		"URL=http://localhost:1080/WebTours/header.html", 
		"Resource=0", 
		"Referer=http://localhost:1080/WebTours/", 
		"Snapshot=t2.inf", 
		"Mode=HTML", 
		LAST);

	web_url("welcome.pl", 
		"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=true", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/WebTours/", 
		"Snapshot=t3.inf", 
		"Mode=HTML", 
		LAST);

	lr_end_transaction("GoTo_LoginPage",LR_AUTO);

	lr_start_transaction("GoTo_RegistrationPage");

	web_revert_auto_header("Upgrade-Insecure-Requests");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(41);

	web_link("sign up now", 
		"Text=sign up now", 
		"Snapshot=t4.inf", 
		LAST);

	lr_end_transaction("GoTo_RegistrationPage",LR_AUTO);

	lr_start_transaction("Registation");

	web_add_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	lr_think_time(89);

	web_submit_form("login.pl", 
		"Snapshot=t5.inf", 
		ITEMDATA, 
		"Name=username", "Value=newname", ENDITEM, 
		"Name=password", "Value=123", ENDITEM, 
		"Name=passwordConfirm", "Value=123", ENDITEM, 
		"Name=firstName", "Value=NewFirstName", ENDITEM, 
		"Name=lastName", "Value=NewLastName", ENDITEM, 
		"Name=address1", "Value=NewStreetAdress", ENDITEM, 
		"Name=address2", "Value=NewCity/NewState/NewZip", ENDITEM, 
		"Name=register.x", "Value=68", ENDITEM, 
		"Name=register.y", "Value=5", ENDITEM, 
		LAST);

	lr_end_transaction("Registation",LR_AUTO);

	lr_start_transaction("Goto_Continue");

	web_revert_auto_header("Sec-Fetch-User");

	web_revert_auto_header("Upgrade-Insecure-Requests");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(45);

	web_image("button_next.gif", 
		"Src=/WebTours/images/button_next.gif", 
		"Snapshot=t6.inf", 
		LAST);

	lr_end_transaction("Goto_Continue",LR_AUTO);

	lr_start_transaction("Logout");

	web_revert_auto_header("Sec-Fetch-User");

	web_add_header("Upgrade-Insecure-Requests", 
		"1");

	lr_think_time(10);

	web_image("SignOff Button", 
		"Alt=SignOff Button", 
		"Snapshot=t7.inf", 
		LAST);

	lr_end_transaction("Logout",LR_AUTO);

	lr_end_transaction("UC6_RegistrationNewUser",LR_AUTO);

	return 0;
}