Action()
{
	//ѕеременна€ в которой хранит€ значение по покупке обратного билета
	int randRoundtrip;
	
	/*ѕарсинг случайной строки из файла с данными пользователей,
	после чего получим параметры пользовател€ (логин, пароль, им€ и т.д)*/
	readRowFromParameters();
	
	//—лучайный выбор требовани€ на обратный рейса 
	randRoundtrip = getRandomRoundtrip();
	
	//‘ункци€ выбирает неодинаковые города отправлени€ и прилета
	getDepartCityAndArriveCity();
	
	lr_start_transaction("UC2_SearchTicketWithoutPayment");
	
	
	lr_start_transaction("Goto_LoginPage");
	
		web_set_sockets_option("SSL_VERSION", "2&3");
		web_add_header("Sec-Fetch-Dest", "document");
		web_add_header("Sec-Fetch-Site", "none");
		web_add_auto_header("Sec-Fetch-Mode", "navigate");
		web_add_header("Sec-Fetch-User", "?1");
		web_add_auto_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=#E0E7F1",LAST);
		
		web_url("WebTours", 
			"URL=http://localhost:1080/WebTours/", 
			"Resource=0", 
			"Referer=", 
			"Snapshot=t8.inf", 
			"Mode=HTML", 
			LAST);
		
		web_add_auto_header("Sec-Fetch-Dest", "frame");
		web_add_auto_header("Sec-Fetch-Site", "same-origin");
		
		web_reg_find("Text=&nbsp;",LAST);
		
		web_url("header.html", 
			"URL=http://localhost:1080/WebTours/header.html", 
			"Resource=0", 
			"Referer=http://localhost:1080/WebTours/", 
			"Snapshot=t9.inf", 
			"Mode=HTML", 
			LAST);
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_url("welcome.pl", 
			"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=true", 
			"Resource=0", 
			"RecContentType=text/html", 
			"Referer=http://localhost:1080/WebTours/", 
			"Snapshot=t10.inf", 
			"Mode=HTML", 
			LAST);
		
	lr_end_transaction("Goto_LoginPage",LR_AUTO);
	
	lr_think_time(10);
	
	lr_start_transaction("Login");
	
		web_add_header("Origin", "http://localhost:1080");
		web_add_auto_header("Sec-Fetch-User", "?1");
		
		web_reg_find("Text=User password was correct",LAST);
		web_reg_find("Text=Welcome, <b>{login}</b>, to the Web Tours",LAST);
		
		web_submit_form("login.pl", 
			"Snapshot=t11.inf", 
			ITEMDATA, 
			"Name=username", "Value={login}", ENDITEM, 
			"Name=password", "Value={password}", ENDITEM, 
			LAST);
		
	lr_end_transaction("Login",LR_AUTO);
	
	lr_think_time(5);
	
	lr_start_transaction("Goto_FindFlightPage");
	
		web_revert_auto_header("Sec-Fetch-User");
		web_revert_auto_header("Upgrade-Insecure-Requests");
		web_add_auto_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=User has returned to the search page",LAST);
		
		web_image("Search Flights Button",
			"Alt=Search Flights Button", 
			"Snapshot=t12.inf", 
			LAST);
		
	lr_end_transaction("Goto_FindFlightPage",LR_AUTO);
	
	lr_think_time(10);
	
	lr_start_transaction("Goto_FoundFlightPage");
	
		web_add_header("Origin", "http://localhost:1080");
		web_add_auto_header("Sec-Fetch-User", "?1");
		
		if (randRoundtrip==1){
			//≈сли установлена покупка обраного билета ищем в ответе данные о рейсах туда-обратно
			web_reg_find("Text=<blockquote>Flight departing from <B>{departCity}</B> to <B>{arriveCity}</B> on <B>{departDate}</B>",LAST);
			web_reg_find("Text=<blockquote>Flight departing from <B>{arriveCity}</B> to <B>{departCity}</B> on <B>{returnDate}</B>",LAST);
		} else{
			//≈сли обратный путь не требуетс€ ищем в ответе данные о рейсе только туда
			web_reg_find("Text=<blockquote>Flight departing from <B>{departCity}</B> to <B>{arriveCity}</B> on <B>{departDate}</B>",LAST);
		}
		
		web_submit_form("reservations.pl", 
			"Snapshot=t13.inf", 
			ITEMDATA, 
			"Name=depart", "Value={departCity}", ENDITEM, 
			"Name=departDate", "Value={departDate}", ENDITEM, 
			"Name=arrive", "Value={arriveCity}", ENDITEM, 
			"Name=returnDate", "Value={returnDate}", ENDITEM, 
			"Name=numPassengers", "Value={randomNumPassengers}", ENDITEM, 
			"Name=roundtrip", "Value={roundtrip}", ENDITEM, 
			"Name=seatPref", "Value={preferenceSeat}", ENDITEM, 
			"Name=seatType", "Value={typeOfSeat}", ENDITEM, 
			"Name=findFlights.x", "Value=11", ENDITEM, 
			"Name=findFlights.y", "Value=7", ENDITEM, 	
			LAST);
		
	lr_end_transaction("Goto_FoundFlightPage",LR_AUTO);
	
	lr_think_time(5);
	
	lr_start_transaction("Logout");
	
		web_revert_auto_header("Sec-Fetch-User");
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_image("SignOff Button", 
			"Alt=SignOff Button", 
			"Snapshot=t14.inf", 
			LAST);
		
	lr_end_transaction("Logout",LR_AUTO);	
	
	
	lr_end_transaction("UC2_SearchTicketWithoutPayment",LR_AUTO);

	return 0;
}


// ак работает описано в UC1
void readRowFromParameters(){
    char login[20];
    char password[20];
    char firstName[20];
	char lastName[20]; 
	char adress1[20];
	char adress2[60];
	char creditCard[20];
	char expirationDate[20];
    char row[400] = "";
    strcpy(row, lr_eval_string("{randomRowLogin}"));
    
    sscanf(row,"%s	%s	%s	%s	%s	%s	%s	%s"
          	,login
    	    ,&(password)
    	    ,&(firstName)
    	    ,&(lastName)
    	    ,&(adress1)
    	    ,&(adress2)
    	    ,&(creditCard)
    	    ,&(expirationDate));
    
	lr_save_string(login, "login");
	lr_save_string(password, "password");
	lr_save_string(firstName, "firstName");
	lr_save_string(lastName, "lastName");
	lr_save_string(adress1, "adress1");
	lr_save_string(adress2, "adress2");
	lr_save_string(creditCard, "creditCard");
	lr_save_string(expirationDate, "expirationDate");
	return;
}



int getRandomRoundtrip(){
	//«аносим в random переменную случайное числовое значение из параметра среды VuGen'a (рандом 1-100)
	int random = atoi(lr_eval_string("{randomRoundtrip}"));
	//«аносим в procentTrue переменную числовое значение из параметра среды VuGen'a 
	//(как часто будет установлен обратный рейс). «начение установлено на 25%
	int procentTrue = atoi(lr_eval_string("{procentTrueRoundtrip}"));
	int tfRoundtrip;
	char roundtripStr[5];
	
	//”станавливаем в соответсвии с веро€тностю покупку обратного билета
	if(random>(100-procentTrue)) {
		strcpy(roundtripStr, "on");
		tfRoundtrip = 1;
	} else{
		strcpy(roundtripStr, "<OFF>");
		tfRoundtrip = 0;
	}
	//«аносим найденное значение в переменную среды VuGen'a
	lr_save_string(roundtripStr, "roundtrip");
	//¬озвращаем значение того, установлена ли покупка обратного билета
	return tfRoundtrip;
}


void getDepartCityAndArriveCity(){
	//ѕеременные дл€ города отправлени€ и прилета
	char departCity[20]="";
	char arriveCity[20]="";
	
	//÷икл выполн€етс€ пока обе переменные одинаковы
	//ѕараметр среды VuGen city беретс€ случайно из файла при каждом вызове параметра
	while(strcmp (departCity, arriveCity)==0){
		strcpy(departCity, "");
		strcpy(arriveCity, "");
		strcpy(departCity, lr_eval_string("{city}"));
		strcpy(arriveCity, lr_eval_string("{city}"));
	}
	//«аносим найденные значени€ в переменные среды VuGen'a
	lr_save_string(departCity, "departCity");
	lr_save_string(arriveCity, "arriveCity");
}
