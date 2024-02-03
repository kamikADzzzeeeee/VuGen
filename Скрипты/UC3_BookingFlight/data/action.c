Action()
{

	lr_start_transaction("000");

	lr_start_transaction("111");

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
		"Snapshot=t15.inf", 
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
		"Snapshot=t16.inf", 
		"Mode=HTML", 
		LAST);

	web_url("welcome.pl", 
		"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=true", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://localhost:1080/WebTours/", 
		"Snapshot=t17.inf", 
		"Mode=HTML", 
		LAST);

	lr_end_transaction("111",LR_AUTO);

	lr_start_transaction("222");

	web_add_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(9);

	web_submit_form("login.pl", 
		"Snapshot=t18.inf", 
		ITEMDATA, 
		"Name=username", "Value=jojo", ENDITEM, 
		"Name=password", "Value=bean", ENDITEM, 
		LAST);

	lr_end_transaction("222",LR_AUTO);

	lr_start_transaction("333");

	web_revert_auto_header("Sec-Fetch-User");

	web_revert_auto_header("Upgrade-Insecure-Requests");

	web_add_auto_header("Upgrade-Insecure-Requests", 
		"1");

	lr_think_time(7);

	web_image("Search Flights Button", 
		"Alt=Search Flights Button", 
		"Snapshot=t19.inf", 
		LAST);

	lr_end_transaction("333",LR_AUTO);

	lr_start_transaction("444");

	web_add_auto_header("Origin", 
		"http://localhost:1080");

	web_add_auto_header("Sec-Fetch-User", 
		"?1");

	lr_think_time(4);

	web_submit_form("reservations.pl", 
		"Snapshot=t20.inf", 
		ITEMDATA, 
		"Name=depart", "Value=Frankfurt", ENDITEM, 
		"Name=departDate", "Value=02/01/2024", ENDITEM, 
		"Name=arrive", "Value=Los Angeles", ENDITEM, 
		"Name=returnDate", "Value=02/02/2024", ENDITEM, 
		"Name=numPassengers", "Value=1", ENDITEM, 
		"Name=roundtrip", "Value=<OFF>", ENDITEM, 
		"Name=seatPref", "Value=None", ENDITEM, 
		"Name=seatType", "Value=Business", ENDITEM, 
		LAST);

	lr_end_transaction("444",LR_AUTO);

	lr_think_time(11);

	lr_start_transaction("555");

	web_submit_form("reservations.pl_2", 
		"Snapshot=t21.inf", 
		ITEMDATA, 
		"Name=outboundFlight", "Value=133;386;02/01/2024", ENDITEM, 
		"Name=reserveFlights.x", "Value=57", ENDITEM, 
		"Name=reserveFlights.y", "Value=6", ENDITEM, 
		LAST);

	lr_end_transaction("555",LR_AUTO);

	lr_think_time(16);

	lr_start_transaction("666");

	web_submit_form("reservations.pl_3", 
		"Snapshot=t22.inf", 
		ITEMDATA, 
		"Name=firstName", "Value=Jojo", ENDITEM, 
		"Name=lastName", "Value=Bean", ENDITEM, 
		"Name=address1", "Value=Komsomolskay, 106/1", ENDITEM, 
		"Name=address2", "Value=Ufa, Bashkortostan, &#1058;&#1058;23", ENDITEM, 
		"Name=pass1", "Value=Jojo Bean", ENDITEM, 
		"Name=creditCard", "Value=123456789", ENDITEM, 
		"Name=expDate", "Value=987", ENDITEM, 
		"Name=saveCC", "Value=<OFF>", ENDITEM, 
		LAST);

	lr_end_transaction("666",LR_AUTO);

	lr_start_transaction("777");

	web_revert_auto_header("Origin");

	web_revert_auto_header("Sec-Fetch-User");

	lr_think_time(7);

	web_image("SignOff Button", 
		"Alt=SignOff Button", 
		"Snapshot=t23.inf", 
		LAST);

	lr_end_transaction("777",LR_AUTO);

	lr_end_transaction("000",LR_AUTO);

	return 0;
}