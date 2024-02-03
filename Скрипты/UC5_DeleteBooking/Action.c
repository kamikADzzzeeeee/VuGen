Action()
{
	int ifNoFlights;
	int countOfFLights;
	int randomFlights;
	int countDeleteFlights;
	int countOfFlightsAfterDelete;
	int tfDeleteAll;
	char countOfFlightsAfterDeleteStr[20];
	char countOfFLightsStr[20];
	char randomFlightsStr[20];
	
	readRowFromParameters();
	tfDeleteAll = getRandomDeleteAll();
	
	lr_start_transaction("UC5_DeleteBooking");
	
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
			"Snapshot=t1.inf", 
			"Mode=HTML", 
			LAST);
		
		web_add_auto_header("Sec-Fetch-Dest", "frame");
		web_add_auto_header("Sec-Fetch-Site", "same-origin");
		
		web_reg_find("Text=&nbsp;",LAST);
		
		web_url("header.html", 
			"URL=http://localhost:1080/WebTours/header.html", 
			"Resource=0", 
			"Referer=http://localhost:1080/WebTours/", 
			"Snapshot=t2.inf", 
			"Mode=HTML", 
			LAST);
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_url("welcome.pl", 
			"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=true", 
			"Resource=0", 
			"RecContentType=text/html", 
			"Referer=http://localhost:1080/WebTours/", 
			"Snapshot=t3.inf", 
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
			"Snapshot=t4.inf", 
			ITEMDATA, 
			"Name=username", "Value={login}", ENDITEM, 
			"Name=password", "Value={password}", ENDITEM, 
			LAST);
		
	lr_end_transaction("Login",LR_AUTO);
	
	lr_think_time(5);
	
	lr_start_transaction("Goto_ItineraryPage");
	
		web_add_auto_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=<title>Flights List</title>",LAST);
		web_reg_find("Text=User wants the intineraries",LAST);
		//¬ ответе ищем кол-во забронированных заказов (если заказов нет, то будет выдан warning, но выполнение продолжитс€)
		web_reg_save_param_regexp("ParamName=countOfFlightsRegexp","RegExp=A total of (.*?) scheduled flights","Notfound=warning",SEARCH_FILTERS,LAST);
		
		web_image("Itinerary Button",
			"Alt=Itinerary Button", 
			"Snapshot=t5.inf", 
			LAST);
		
	lr_end_transaction("Goto_ItineraryPage",LR_AUTO);
	
	//¬ числовую переменную заносим кол-во заказов	
	countOfFLights = atoi(lr_eval_string("{countOfFlightsRegexp}"));
	itoa(countOfFLights, countOfFLightsStr, 10);
	lr_save_string(countOfFLightsStr, "countOfFlights");
	
	//≈сли кол-во брони больше 0, то будут выполн€тьс€ транзакции на удалени€
	//≈сли же кол-во брони = 0, то сразу будет Logout
	if (countOfFLights!=0) {
		
		//¬ыбираем случайное кол-во рейсов дл€ удалени€(рейсы дл€ удалени€ также выбираютс€ случайно)
		countDeleteFlights = deleteRandomTickets(countOfFLights);
		//¬ычисл€ем сколько рейсов будет после удалени€
		countOfFlightsAfterDelete = countOfFLights - countDeleteFlights;
		itoa(countOfFlightsAfterDelete,countOfFlightsAfterDeleteStr,10);
		lr_save_string(countOfFlightsAfterDeleteStr, "countOfFlightsAfterDelete");
		
		//ѕроверка на то, что выпала веро€тность удалить сразу ве билеты
		if (tfDeleteAll==0) {
			
			lr_think_time(10);
			
			//≈сли веро€тность не выпала - удал€ем по несколько 
			lr_start_transaction("DeleteFlight");
			
				web_add_auto_header("Origin", "http://localhost:1080");
				web_add_auto_header("Sec-Fetch-User", "?1");
				
				web_reg_find("Text=<title>Flights List</title>",LAST);
				
				//¬ зависимости от количества билетов после удалени€
				//ѕровер€ем в ответе необходимый текст
				if (countOfFlightsAfterDelete==0) {
					web_reg_find("Text=No flights have been reserved",LAST);
				} else {
					web_reg_find("Text=A total of {countOfFlightsAfterDelete} scheduled flights",LAST);
				}
				
				web_submit_form("itinerary.pl", 
					"Snapshot=t6.inf", 
					ITEMDATA, 
					"Name=removeFlights.x", "Value=11", ENDITEM, 
					"Name=removeFlights.y", "Value=7", ENDITEM, 
					"{del1}", "Value={value1}", ENDITEM,
					"{del2}", "Value={value2}", ENDITEM,
					"{del3}", "Value={value3}", ENDITEM,
					"{del4}", "Value={value4}", ENDITEM,
					"{del5}", "Value={value5}", ENDITEM,
					"{del6}", "Value={value6}", ENDITEM,
					"{del7}", "Value={value7}", ENDITEM,
					"{del8}", "Value={value8}", ENDITEM,
					"{del9}", "Value={value9}", ENDITEM,
					"{del10}", "Value={value10}", ENDITEM,
					LAST);
				
			lr_end_transaction("DeleteFlight",LR_AUTO);
	
		} else {
			
			lr_think_time(10);
			
			//≈сли веро€тность выпала - удал€ем сразу все билеты
			lr_start_transaction("DeleteAllFlight");
				
				web_reg_find("Text=No flights have been reserved",LAST);
				
				web_submit_form("itinerary.pl_2", 
					"Snapshot=t7.inf", 
					ITEMDATA, 
					"Name=removeAllFlights.x", "Value=11", ENDITEM, 
					"Name=removeAllFlights.y", "Value=7", ENDITEM,
					LAST);
				
			lr_end_transaction("DeleteAllFlight",LR_AUTO);	
			
		}
	}
	
	lr_think_time(5);
	
	lr_start_transaction("Logout");
	
		web_add_header("Sec-Fetch-User", "?1");
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_image("SignOff Button", 
			"Alt=SignOff Button", 
			"Snapshot=t8.inf", 
			LAST);
		
	lr_end_transaction("Logout",LR_AUTO);
	
	lr_end_transaction("UC5_DeleteBooking",LR_AUTO);

	return 0;
}


//«аполн€ем параметры дл€ удалени€ брони
int deleteRandomTickets(int countOfFLights){
	
	int randomDeleteFlights;
	char randomDeleteFlightsStr[10];
	char del[20];
	int i;
	char colDeleteFlightsStr[20];
	int hh1=0;
	int j = 0;
	int colDeleteFlights = 1 + rand()%(10);
	int hhh[10];
	
	itoa(colDeleteFlights, colDeleteFlightsStr, 10);
	
	lr_save_string(colDeleteFlightsStr, "colDeleteFlights");
	
	lr_save_string("LAST", "del1");
	lr_save_string("<OFF>", "value1");
	lr_save_string("LAST", "del2");
	lr_save_string("<OFF>", "value2");
	lr_save_string("LAST", "del3");
	lr_save_string("<OFF>", "value3");
	lr_save_string("LAST", "del4");
	lr_save_string("<OFF>", "value4");
	lr_save_string("LAST", "del5");
	lr_save_string("<OFF>", "value5");
	lr_save_string("LAST", "del6");
	lr_save_string("<OFF>", "value6");
	lr_save_string("LAST", "del7");
	lr_save_string("<OFF>", "value7");
	lr_save_string("LAST", "del8");
	lr_save_string("<OFF>", "value8");
	lr_save_string("LAST", "del9");
	lr_save_string("<OFF>", "value9");
	lr_save_string("LAST", "del10");
	lr_save_string("<OFF>", "value10");
	
	for (i=1; i<=colDeleteFlights; i++){
		
		strcpy(del,"");
		strcat(del, "Name\=");
		hh1=0;
		
		//цикл дл€ того, чтобы бронь не повтор€лась
		while (hh1==0){
			randomDeleteFlights = 1 + rand()%(countOfFLights);
			hh1=1;
			for (j=0;j<i;j++){
				if (hhh[j]==randomDeleteFlights){
					hh1=0;
				}
			}

		}
		
		hhh[i-1] = randomDeleteFlights;
		
		
		if (i==1) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del1");
			lr_save_string("on", "value1");
		}
		if (i==2) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del2");
			lr_save_string("on", "value2");
		}
		if (i==3) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del3");
			lr_save_string("on", "value3");
		}
		if (i==4) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del4");
			lr_save_string("on", "value4");
		}
		if (i==5) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del5");
			lr_save_string("on", "value5");
		}
		if (i==6) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del6");
			lr_save_string("on", "value6");
		}
		if (i==7) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del7");
			lr_save_string("on", "value7");
		}
		if (i==8) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del8");
			lr_save_string("on", "value8");
		}
		if (i==9) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del9");
			lr_save_string("on", "value9");
		}
		if (i==10) {
			itoa(randomDeleteFlights, randomDeleteFlightsStr, 10);
			strcat(del, randomDeleteFlightsStr);
			lr_save_string(del, "del10");
			lr_save_string("on", "value10");
		}

	}
	
	return colDeleteFlights;
	
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

}

// ак работает описано в UC2 (аналогично randomRoundtrip)
//5% что будет удалены все рейсы. ¬ оставльных случае€х рейсы будут удал€тьс€ рандомно
int getRandomDeleteAll(){
	int random = atoi(lr_eval_string("{randomDeleteAll}"));
	int procentTrue = atoi(lr_eval_string("{procentTrueDeleteAll}"));
	int tfDelAll;
			
	if(random>(100-procentTrue)) {
		lr_save_string("true", "DeleteAll");
		tfDelAll = 1;
	} else{
		lr_save_string("false", "DeleteAll");
		tfDelAll = 0;
	}
	
	return tfDelAll;
}