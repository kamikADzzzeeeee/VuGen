Action()
{

	lr_start_transaction("UC5_DeleteBooking");

	lr_start_transaction("Goto_LoginPage");

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

	lr_end_transaction("Goto_LoginPage",LR_AUTO);

	lr_start_transaction("Login");

	web_add_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(13);

	web_submit_form("login.pl", 
		"Snapshot=t4.inf", 
		ITEMDATA, 
		"Name=username", "Value=jojo", ENDITEM, 
		"Name=password", "Value=bean", ENDITEM, 
		LAST);

	lr_end_transaction("Login",LR_AUTO);

	lr_start_transaction("Goto_ItineraryPage");

	web_revert_auto_header("Sec-Fetch-User");

	web_revert_auto_header("Upgrade-Insecure-Requests");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	lr_think_time(18);

	web_image("Itinerary Button", 
		"Alt=Itinerary Button", 
		"Snapshot=t5.inf", 
		LAST);

	lr_end_transaction("Goto_ItineraryPage",LR_AUTO);

	lr_start_transaction("DeleteFlight");

	web_add_auto_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(7);

	web_submit_form("itinerary.pl", 
		"Snapshot=t6.inf", 
		ITEMDATA, 
		"Name=1", "Value=on", ENDITEM, 
		"Name=2", "Value=on", ENDITEM, 
		"Name=3", "Value=on", ENDITEM, 
		"Name=4", "Value=<OFF>", ENDITEM, 
		"Name=5", "Value=<OFF>", ENDITEM, 
		"Name=6", "Value=<OFF>", ENDITEM, 
		"Name=7", "Value=<OFF>", ENDITEM, 
		"Name=8", "Value=<OFF>", ENDITEM, 
		LAST);

	lr_end_transaction("DeleteFlight",LR_AUTO);

	lr_think_time(11);

	lr_start_transaction("DeleteAllFlight");

	web_submit_form("itinerary.pl_2", 
		"Snapshot=t7.inf", 
		ITEMDATA, 
		"Name=1", "Value=<OFF>", ENDITEM, 
		"Name=2", "Value=<OFF>", ENDITEM, 
		"Name=3", "Value=<OFF>", ENDITEM, 
		"Name=4", "Value=<OFF>", ENDITEM, 
		"Name=5", "Value=<OFF>", ENDITEM, 
		LAST);

	lr_end_transaction("DeleteAllFlight",LR_AUTO);

	lr_start_transaction("Logout");

	web_revert_auto_header("Origin");

	web_revert_auto_header("Sec-Fetch-User");

	web_add_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(10);

	web_image("SignOff Button", 
		"Alt=SignOff Button", 
		"Snapshot=t8.inf", 
		LAST);

	lr_end_transaction("Logout",LR_AUTO);

	lr_end_transaction("UC5_DeleteBooking",LR_AUTO);

	return 0;
}