Action()
{
	//Переменная в которой хранится значение по покупке обратного билета
	int randRoundtrip;
	
	//Переменная в которой хранится случайное значение количества пассажиров (от 1 до 10)
	int colPass = atoi(lr_eval_string("{randomNumPassengers}"));
	
	/*Парсинг случайной строки из файла с данными пользователей,
	после чего получим параметры пользователя (логин, пароль, имя и т.д)*/
	readRowFromParameters();
	
	//Случайный выбор требования на обратный рейса 
	randRoundtrip = getRandomRoundtrip();
	
	//Случайный выбор требования на сохранение данных о кредитной карте
	getRandomSaveCC();
	
	//Функция выбирает неодинаковые города отправления и прилета
	getDepartCityAndArriveCity();
	
	//Функция для установки параметров для отправки данных о пассажирах 
	getLastPassenger();
	
	
	lr_start_transaction("UC3_BookingFlight");
		
	
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
			"Snapshot=t15.inf", 
			"Mode=HTML", 
			LAST);
		
		web_add_auto_header("Sec-Fetch-Dest", "frame");
		web_add_auto_header("Sec-Fetch-Site", "same-origin");
		
		web_reg_find("Text=&nbsp;",LAST);
		
		web_url("header.html", 
			"URL=http://localhost:1080/WebTours/header.html", 
			"Resource=0", 
			"Referer=http://localhost:1080/WebTours/", 
			"Snapshot=t16.inf", 
			"Mode=HTML", 
			LAST);
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_url("welcome.pl", 
			"URL=http://localhost:1080/cgi-bin/welcome.pl?signOff=true", 
			"Resource=0", 
			"RecContentType=text/html", 
			"Referer=http://localhost:1080/WebTours/", 
			"Snapshot=t17.inf", 
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
			"Snapshot=t18.inf", 
			ITEMDATA, 
			"Name=username", "Value={login}", ENDITEM, 
			"Name=password", "Value={password}", ENDITEM, 
			LAST);
		
	lr_end_transaction("Login",LR_AUTO);
	
	lr_think_time(10);
	
	lr_start_transaction("Goto_FindFlightPage");
	
		web_revert_auto_header("Sec-Fetch-User");
		web_revert_auto_header("Upgrade-Insecure-Requests");
		web_add_auto_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=User has returned to the search page",LAST);
		
		web_image("Search Flights Button",
			"Alt=Search Flights Button", 
			"Snapshot=t19.inf", 
			LAST);
		
	lr_end_transaction("Goto_FindFlightPage",LR_AUTO);
	
	lr_think_time(10);
	
	lr_start_transaction("Goto_FoundFlightPage");
	
		web_add_header("Origin", "http://localhost:1080");
		web_add_auto_header("Sec-Fetch-User", "?1");
		
		if (randRoundtrip==1){
			//Если установлена покупка обраного билета ищем в ответе данные о рейсах туда-обратно
			web_reg_find("Text=<blockquote>Flight departing from <B>{departCity}</B> to <B>{arriveCity}</B> on <B>{departDate}</B>",LAST);
			web_reg_find("Text=<blockquote>Flight departing from <B>{arriveCity}</B> to <B>{departCity}</B> on <B>{returnDate}</B>",LAST);
			//Если установлена покупка обраного билета случайно выбираем и сохраняем рейс туда и рейс обратно
			web_reg_save_param_regexp("ParamName=randomOutboundFlight","RegExp=\<input type\=\"radio\" name\=\"outboundFlight\" value\=\"(.*?)\"","Ordinal={randomFlight}",SEARCH_FILTERS,LAST);
			web_reg_save_param_regexp("ParamName=randomReturnFlight","RegExp=\<input type\=\"radio\" name\=\"returnFlight\" value\=\"(.*?)\"","Ordinal={randomFlight}",SEARCH_FILTERS,LAST);
		} else{
			//Если обратный путь не требуется ищем в ответе данные о рейсе только туда
			web_reg_find("Text=<blockquote>Flight departing from <B>{departCity}</B> to <B>{arriveCity}</B> on <B>{departDate}</B>",LAST);
			//Если обратный путь не требуется случайно выбираем и сохраняем рейс туда
			web_reg_save_param_regexp("ParamName=randomOutboundFlight","RegExp=\<input type\=\"radio\" name\=\"outboundFlight\" value\=\"(.*?)\"","Ordinal={randomFlight}",SEARCH_FILTERS,LAST);
		}
		
		web_submit_form("reservations.pl", 
			"Snapshot=t20.inf", 
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
	
	//Парсим строку с случайно выбранным рейсом
	//Функция сразу считает полную стоимость заказа
	getDataFromOutRetFlight(randRoundtrip);
	lr_think_time(15);
	
	lr_start_transaction("Goto_PaymentDetailsPage");
	
		//Если требуется устанавливаем в запросе данные о возвратном рейсе
		if (randRoundtrip==1){
			lr_save_string("Name\=returnFlight", "returnFlight");
		} else{
			lr_save_string("LAST", "returnFlight");
		}
		
		web_reg_find("Text=Payment Details",LAST);
		web_reg_find("Text=<title>Flight Reservation</title>",LAST);
		//Проверяем, что оплачиваем нужное количество пассажиров и нужное количество денег (включая стоимость обратого пути, если он установлен)
		web_reg_find("Text=Total for {randomNumPassengers} ticket(s) is \= $ {fullPrice}",LAST);
		
		web_submit_form("reservations.pl_2",
		    "Snapshot=t21.inf",
			ITEMDATA, 
			"Name=reserveFlights.x", "Value=11", ENDITEM, 
			"Name=reserveFlights.y", "Value=7", ENDITEM, 
			"Name={outboundFlight}", "Value={randomOutboundFlight}", ENDITEM, 
			"{returnFlight}", "Value={randomReturnFlight}", ENDITEM, 
			LAST);
		
	lr_end_transaction("Goto_PaymentDetailsPage",LR_AUTO);
	
	lr_think_time(20);
	
	lr_start_transaction("Goto_InvoicePage");
	
		if (randRoundtrip==1){
			//Если установлена покупка обраного билета ищем в ответе данные о рейсах туда-обратно
			web_reg_find("Text=Flight {outboundNumberFlight} leaves {departCity}  for {arriveCity}",LAST);
			web_reg_find("Text=Flight {returnNumberFlight} leaves {arriveCity}  for {departCity}",LAST);
			
		} else{
			//Если не установлена покупка обраного билета ищем в ответе данные о рейсе туда
			web_reg_find("Text=Flight {outboundNumberFlight} leaves {departCity}  for {arriveCity}",LAST);
		}
	
		//В зависомости от кол-ва пассажиров проверяем в ответе разные строки
		if (colPass==1){
			web_reg_find("Text=A {typeOfSeat} Class",LAST);
			web_reg_find("Text=from {departCity} to {arriveCity}",LAST);
		} else {
			web_reg_find("Text={randomNumPassengers} {typeOfSeat} Class tickets from {departCity} to {arriveCity}.",LAST);
		}
		
		//Проверяем в ответе данные о кредитной карте
		web_reg_find("Text=Total Charged to Credit Card # {creditCard}",LAST);
		
		//Проверяем в ответе полную стоимость заказа
		web_reg_find("Text={fullPrice}",LAST);
		web_reg_find("Text=Thank you for booking through Web Tours",LAST);
		
		
		web_submit_form("reservations.pl_3", 
			"Snapshot=t22.inf", 
			ITEMDATA, 
			"Name=firstName", "Value={firstName}", ENDITEM, 
			"Name=lastName", "Value={lastName}", ENDITEM, 
			"Name=address1", "Value={adress1}", ENDITEM, 
			"Name=address2", "Value={adress2}", ENDITEM, 
			"Name=creditCard", "Value={creditCard}", ENDITEM, 
			"Name=expDate", "Value={expirationDate}", ENDITEM, 
			"Name=saveCC", "Value={saveCC}", ENDITEM,
			"Name=buyFlights.x", "Value=11", ENDITEM, 
			"Name=buyFlights.y", "Value=7", ENDITEM, 
			"Name=pass1", "Value={firstName} {lastName}", ENDITEM, 	
			"{pass2}", "Value={value2}", ENDITEM, 
			"{pass3}", "Value={value3}", ENDITEM,
			"{pass4}", "Value={value4}", ENDITEM, 
			"{pass5}", "Value={value5}", ENDITEM,
			"{pass6}", "Value={value6}", ENDITEM, 
			"{pass7}", "Value={value7}", ENDITEM,
			"{pass8}", "Value={value8}", ENDITEM, 
			"{pass9}", "Value={value9}", ENDITEM,
			"{pass10}", "Value={value10}", ENDITEM, 
			LAST);
		
	lr_end_transaction("Goto_InvoicePage",LR_AUTO);
	
	lr_think_time(5);
	
	lr_start_transaction("Logout");
	
		web_revert_auto_header("Sec-Fetch-User");
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_image("SignOff Button",
			"Alt=SignOff Button", 
			"Snapshot=t23.inf", 
			LAST);
		
	lr_end_transaction("Logout",LR_AUTO);

	
	lr_end_transaction("UC3_BookingFlight",LR_AUTO);

	return 0;
}

//Как работает описано в UC2
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

//Как работает описано в UC2
int getRandomRoundtrip(){
	int random = atoi(lr_eval_string("{randomRoundtrip}"));
	int procentTrue = atoi(lr_eval_string("{procentTrueRoundtrip}"));
	int tfRoundtrip;
	char roundtripStr[5];
			
	if(random>(100-procentTrue)) {
		strcpy(roundtripStr, "on");
		tfRoundtrip = 1;
	} else{
		strcpy(roundtripStr, "<OFF>");
		tfRoundtrip = 0;
	}
	lr_save_string(roundtripStr, "roundtrip");
	return tfRoundtrip;
}

//Как работает описано в UC2 (аналогично функции getRandomRoundtrip)
//Вероятность сохранения данных о карте составляет 5%
void getRandomSaveCC(){
	int random = atoi(lr_eval_string("{randomSaveCC}"));
	int procentTrue = atoi(lr_eval_string("{procentTruetfSaveCC}"));
	int tfSaveCC;
			
	if(random>(100-procentTrue)) {
		lr_save_string("on", "saveCC");
	} else{
		lr_save_string("<OFF>", "saveCC");
	}
}

//Как работает описано в UC2
void getDepartCityAndArriveCity(){
	char departCity[20]="";
	char arriveCity[20]="";
	
	while(strcmp (departCity, arriveCity)==0){
		strcpy(departCity, "");
		strcpy(arriveCity, "");
		strcpy(departCity, lr_eval_string("{city}"));
		strcpy(arriveCity, lr_eval_string("{city}"));
	}
	lr_save_string(departCity, "departCity");
	lr_save_string(arriveCity, "arriveCity");
}

void getDataFromOutRetFlight(int roundtrip){ 
	//Инициализация переменных
	char randomOutboundFlight[30]="";
	char randomReturnFlight[30]="";  
	char sep[10]=";";
	char *istr1="";
	char *istr2="";
	int outboundPrice = 0;	
	int returnPrice = 0;
	int colPassengers = 0;
	int fullPrice = 0;	
	char fullPriceStr[25];
	int i = 0;
	
	/*
	Строки со случайным выбранным рейсом имеют формат: 852;1427;02/02/2024
	852 		- номер самолета
	1427 		- стоимость
	02/02/2024 	- дата вылета
	 */
	
	//Заносим в переменную строку с случайно выбранным рейсом отправления
	strcpy(randomOutboundFlight, lr_eval_string("{randomOutboundFlight}"));
	//Заносим в переменную строку с случайно выбранным рейсом обратного отправления
	strcpy(randomReturnFlight, lr_eval_string("{randomReturnFlight}"));
	
	//Заносим в переменную числовое значение случайно выбранного кол-ва пассажиров
	colPassengers = atoi(lr_eval_string("{randomNumPassengers}")); 
	
	//Если установлен обратный рейс, то парсим две строки, если нет - одну
	if (roundtrip==1) {
		istr1 = (char *)strtok(randomOutboundFlight,sep);
		while ((istr1 != NULL)){
			if (i==0) {
				lr_save_string(istr1, "outboundNumberFlight");
			}
			if (i==1) {
				lr_save_string(istr1, "outboundPriceFlight");
			}
			if (i==2) {
				lr_save_string(istr1, "outboundDataFlight");
			}
			i++;
			istr1 = (char *)strtok(NULL,sep);
		}
		i = 0;
		istr2 = (char *)strtok(randomReturnFlight,sep);
		while ((istr2 != NULL)){
			if (i==0) {
				lr_save_string(istr2, "returnNumberFlight");
			}
			if (i==1) {
				lr_save_string(istr2, "returnPriceFlight");
			}
			if (i==2) {
				lr_save_string(istr2, "returnDataFlight");
			}
			i++;
			istr2 = (char *)strtok(NULL,sep);
		}
		
		//Сразу считаем полную стоимость заказа и заносим ее в параметры среды VuGen'a (для туда-обратно)
		outboundPrice = atoi(lr_eval_string("{outboundPriceFlight}"));	
		returnPrice = atoi(lr_eval_string("{returnPriceFlight}"));
		fullPrice = (outboundPrice+returnPrice)*colPassengers;
		itoa(fullPrice, fullPriceStr, 10);
		lr_save_string(fullPriceStr, "fullPrice");
	} else {
		istr1 = (char *)strtok(randomOutboundFlight,sep);
		while ((istr1 != NULL)){
			if (i==0) {
				lr_save_string(istr1, "outboundNumberFlight");
			}
			if (i==1) {
				lr_save_string(istr1, "outboundPriceFlight");
			}
			if (i==2) {
				lr_save_string(istr1, "outboundDataFlight");
			}
			i++;
			istr1 = (char *)strtok(NULL,sep);
		}
		//Сразу считаем полную стоимость заказа и заносим ее в параметры среды VuGen'a (для туда)
		outboundPrice = atoi(lr_eval_string("{outboundPriceFlight}"));	
		fullPrice = (outboundPrice)*colPassengers;
		itoa(fullPrice, fullPriceStr, 10);
		lr_save_string(fullPriceStr, "fullPrice");
	}

	return;
}


int getLastPassenger(){
	
	/*
	функция web_submit_form завершает свою работу 
	когда после ENDITEM стоит LAST
	
	поэтому функция getLastPassenger устанавливает
	после необходимого количества пассажиров LAST 
	тем самым web_submit_form будет отправлено необходимое кол-во пассажиров
	*/
	
	
	//Заносим в переменную числовое значение случайно выбранного кол-ва пассажиров
	int colPassengers = atoi(lr_eval_string("{randomNumPassengers}")); 
	
	lr_save_string("Name\=pass2", "pass2");
	lr_save_string("Name\=pass3", "pass3");
	lr_save_string("Name\=pass4", "pass4");
	lr_save_string("Name\=pass5", "pass5");
	lr_save_string("Name\=pass6", "pass6");
	lr_save_string("Name\=pass7", "pass7");
	lr_save_string("Name\=pass8", "pass8");
	lr_save_string("Name\=pass9", "pass9");
	lr_save_string("Name\=pass10", "pass10");
	
	//Случайное заполнение имен пассажиров
	lr_save_string(lr_eval_string("{femaleFirstNamePasenger} {lastNamePasenger}a"), "value2");
	lr_save_string(lr_eval_string("{maleFirstNamePasenger} {lastNamePasenger}"), "value3");
	lr_save_string(lr_eval_string("{femaleFirstNamePasenger} {lastNamePasenger}a"), "value4");
	lr_save_string(lr_eval_string("{maleFirstNamePasenger} {lastNamePasenger}"), "value5");
	lr_save_string(lr_eval_string("{femaleFirstNamePasenger} {lastNamePasenger}a"), "value6");
	lr_save_string(lr_eval_string("{maleFirstNamePasenger} {lastNamePasenger}"), "value7");
	lr_save_string(lr_eval_string("{femaleFirstNamePasenger} {lastNamePasenger}a"), "value8");
	lr_save_string(lr_eval_string("{maleFirstNamePasenger} {lastNamePasenger}"), "value9");
	lr_save_string(lr_eval_string("{femaleFirstNamePasenger} {lastNamePasenger}a"), "value10");
	
	
	
	if (colPassengers==1) {
		lr_save_string("LAST", "pass2");
	}
	if (colPassengers==2) {
		lr_save_string("LAST", "pass3");
	}
	if (colPassengers==3) {
		lr_save_string("LAST", "pass4");
	}
	if (colPassengers==4) {
		lr_save_string("LAST", "pass5");
	}
	if (colPassengers==5) {
		lr_save_string("LAST", "pass6");
	}
	if (colPassengers==6) {
		lr_save_string("LAST", "pass7");
	}
	if (colPassengers==7) {
		lr_save_string("LAST", "pass8");
	}
	if (colPassengers==8) {
		lr_save_string("LAST", "pass9");
	}
	if (colPassengers==9) {
		lr_save_string("LAST", "pass10");
	}
	return colPassengers;

	
	web_submit_form(ENDITEM, LAST);
	
	
}