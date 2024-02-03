Action()
{
	/*Парсинг случайной строки из файла с данными пользователей,
	после чего получим параметры пользователя (логин, пароль, имя и т.д)*/
	readRowFromParameters();
	
	lr_start_transaction("UC1_LoginAndLogout");
	
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
	
		web_add_auto_header("Sec-Fetch-Dest", "frame");
		web_revert_auto_header("Upgrade-Insecure-Requests");
		web_add_header("Origin", "http://localhost:1080");
		web_add_auto_header("Sec-Fetch-Site", "same-origin");
		
		web_reg_find("Text=User password was correct",LAST);
		web_reg_find("Text=Welcome, <b>{login}</b>, to the Web Tours",LAST);
		
		web_submit_form("login.pl", 
			"Snapshot=t10.inf", 
			ITEMDATA, 
			"Name=username", "Value={login}", ENDITEM, 
			"Name=password", "Value={password}", ENDITEM, 
			LAST);
		
	lr_end_transaction("Login",LR_AUTO);
	
	lr_think_time(10);
	
	lr_start_transaction("Logout");
	
		web_add_header("Sec-Fetch-User", "?1");
		web_add_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_image("SignOff Button", 
			"Alt=SignOff Button", 
			"Snapshot=t11.inf", 
			LAST);
		
	lr_end_transaction("Logout",LR_AUTO);	
	
	lr_end_transaction("UC1_LoginAndLogout",LR_AUTO);

	return 0;
}

	

void readRowFromParameters(){
	
	//инициализация переменных
    char login[20];
    char password[20];
    char firstName[20];
	char lastName[20]; 
	char adress1[20];
	char adress2[60];
	char creditCard[20];
	char expirationDate[20];
    char row[400] = "";
    //копируем в переменную row случайную строку из файла с данными пользователей
    strcpy(row, lr_eval_string("{randomRowLogin}"));
    
    //парсим строку и заносим данные в переменные (разделитель - TAB)
    sscanf(row,"%s	%s	%s	%s	%s	%s	%s	%s"
          	,login
    	    ,&(password)
    	    ,&(firstName)
    	    ,&(lastName)
    	    ,&(adress1)
    	    ,&(adress2)
    	    ,&(creditCard)
    	    ,&(expirationDate));
    
    //инициализируем переменные в параметрах скрипта
	lr_save_string(login, "login");
	lr_save_string(password, "password");
	lr_save_string(firstName, "firstName");
	lr_save_string(lastName, "lastName");
	lr_save_string(adress1, "adress1");
	lr_save_string(adress2, "adress2");
	lr_save_string(creditCard, "creditCard");
	lr_save_string(expirationDate, "expirationDate");

}
