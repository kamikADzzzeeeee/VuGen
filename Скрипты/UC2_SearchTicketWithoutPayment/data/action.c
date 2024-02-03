Action()
{

	lr_start_transaction("UC2_SearchTicketWithoutPayment");

	lr_start_transaction("GoTo_LoginPage");

	web_set_sockets_option("SSL_VERSION", "2&3");

	web_add_auto_header("Sec-Fetch-Dest", 
		"document");

	web_add_auto_header("Sec-Fetch-Site", 
		"none");

	web_add_auto_header("Sec-Fetch-Mode", 
		"navigate");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	web_url("WebTours", 
		"URL=http://localhost:1080/WebTours/", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t1.inf", 
		"Mode=HTML", 
		LAST);

	lr_end_transaction("GoTo_LoginPage",LR_AUTO);

	lr_start_transaction("Login");

	web_revert_auto_header("Sec-Fetch-User");

	web_add_auto_header("Sec-Fetch-Dest", 
		"frame");

	web_revert_auto_header("Upgrade-Insecure-Requests");

	web_add_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Sec-Fetch-Site", 
		"same-origin");

	lr_think_time(15);

	web_submit_form("login.pl", 
		"Snapshot=t2.inf", 
		ITEMDATA, 
		"Name=username", "Value=jojo", ENDITEM, 
		"Name=password", "Value=bean", ENDITEM, 
		LAST);

	lr_end_transaction("Login",LR_AUTO);

	lr_start_transaction("Goto_FindFlightPage");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	lr_think_time(8);

	web_image("Search Flights Button", 
		"Alt=Search Flights Button", 
		"Snapshot=t3.inf", 
		LAST);

	lr_end_transaction("Goto_FindFlightPage",LR_AUTO);

	lr_start_transaction("Goto_FoundFlightPage");

	web_add_header("Origin", 
		"http://localhost:1080");

	lr_think_time(28);

	web_submit_form("reservations.pl", 
		"Snapshot=t4.inf", 
		ITEMDATA, 
		"Name=depart", "Value=Frankfurt", ENDITEM, 
		"Name=departDate", "Value=01/31/2024", ENDITEM, 
		"Name=arrive", "Value=Seattle", ENDITEM, 
		"Name=returnDate", "Value=02/01/2024", ENDITEM, 
		"Name=numPassengers", "Value=3", ENDITEM, 
		"Name=roundtrip", "Value=on", ENDITEM, 
		"Name=seatPref", "Value=None", ENDITEM, 
		"Name=seatType", "Value=Business", ENDITEM, 
		LAST);

	lr_end_transaction("Goto_FoundFlightPage",LR_AUTO);

	lr_start_transaction("Logout");

	web_revert_auto_header("Sec-Fetch-User");

	web_add_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(19);

	web_image("SignOff Button", 
		"Alt=SignOff Button", 
		"Snapshot=t5.inf", 
		LAST);

	lr_end_transaction("Logout",LR_AUTO);

	lr_end_transaction("UC2_SearchTicketWithoutPayment",LR_AUTO);

	return 0;
}