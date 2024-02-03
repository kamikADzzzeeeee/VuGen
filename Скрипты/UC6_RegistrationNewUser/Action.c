Action()
{
	//Генерация Имени и Фамилии
	getFirstAndLastNameUser();
	
	lr_start_transaction("UC6_RegistrationNewUser");
	
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
	
	lr_start_transaction("Goto_RegistrationPage");
	
		web_revert_auto_header("Upgrade-Insecure-Requests");
		web_add_auto_header("Sec-Fetch-User", "?1");
		
		web_reg_find("Text=First time registering? Please complete the form below",LAST);
		web_reg_find("Text=<title>User Information</title>",LAST);
		
		web_link("sign up now", 
			"Text=sign up now", 
			"Snapshot=t4.inf", 
			LAST);
		
	lr_end_transaction("Goto_RegistrationPage",LR_AUTO);
	
	lr_think_time(60);
	
	lr_start_transaction("Registation");
	
		web_add_header("Origin", "http://localhost:1080");
		web_add_auto_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=<blockquote>Thank you, <b>{firstName}{numberUsername}</b>, for registering and",LAST);
		
		web_submit_form("login.pl", 
			"Snapshot=t5.inf", 
			ITEMDATA, 
			"Name=username", "Value={firstName}{numberUsername}", ENDITEM, 
			"Name=password", "Value={standartPassword}", ENDITEM, 
			"Name=passwordConfirm", "Value={standartPassword}", ENDITEM, 
			"Name=firstName", "Value={firstName}", ENDITEM, 
			"Name=lastName", "Value={lastName}", ENDITEM, 
			"Name=address1", "Value={streetReg}", ENDITEM, 
			"Name=address2", "Value={cityReg}/{stateReg}/{zipCodeReg}", ENDITEM, 
			"Name=register.x", "Value=68", ENDITEM, 
			"Name=register.y", "Value=5", ENDITEM, 
			LAST);
		
	lr_end_transaction("Registation",LR_AUTO);
	
	lr_think_time(5);

	lr_start_transaction("Goto_HomePage");
	
		web_revert_auto_header("Sec-Fetch-User");
		web_revert_auto_header("Upgrade-Insecure-Requests");
		web_add_auto_header("Sec-Fetch-User", "?1");
		
		web_reg_find("Text=Welcome, <b>{firstName}{numberUsername}</b>, to the Web Tours reservation pages",LAST);
		web_reg_find("Text=User has returned to the home page",LAST);
		
		web_image("button_next.gif", 
			"Src=/WebTours/images/button_next.gif", 
			"Snapshot=t6.inf", 
			LAST);
		
	lr_end_transaction("Goto_HomePage",LR_AUTO);
	
	lr_think_time(5);
	
	lr_start_transaction("Logout");
	
		web_revert_auto_header("Sec-Fetch-User");
		web_add_header("Upgrade-Insecure-Requests", "1");
		
		web_reg_find("Text=A Session ID has been created and loaded into a cookie called MSO",LAST);
		
		web_image("SignOff Button", 
			"Alt=SignOff Button", 
			"Snapshot=t7.inf", 
			LAST);
		
	lr_end_transaction("Logout",LR_AUTO);
	
	lr_end_transaction("UC6_RegistrationNewUser",LR_AUTO);

	//Сохранение зарегистрированных пользоватлей в файл
	//Из данного файла данные считываютя остальными скриптами
	writeIntoFile();
	
	return 0;
}


//Генерация Имени и Фамилии для мужчин и женщин
void getFirstAndLastNameUser(){
	int random = atoi(lr_eval_string("{randomNumberForUserGender}"));
	char firstName[20];
	char lastName[20];
	
	char maleFirstName[20];
	char femaleFirstName[20];
	char fmlastName[20];
	char lastName_a[2];
	
	strcpy(femaleFirstName, lr_eval_string("{femaleFirstName}"));
	strcpy(maleFirstName,lr_eval_string("{maleFirstName}"));
	strcpy(fmlastName,lr_eval_string("{ffmmlastName}"));
	
	if(random!=0) {
		strcpy(firstName, femaleFirstName);
		strcpy(lastName_a, "a");
		strcpy(lastName, strcat(fmlastName, lastName_a));
	} else{
		strcpy(firstName, maleFirstName);
		strcpy(lastName, fmlastName);
	}
	
	lr_save_string(firstName, "firstName");
	lr_save_string(lastName, "lastName");
}

//Запись в файл зарегистрированных пользователей
void writeIntoFile(){
	FILE *fp = fopen("..\\Outputfile\\login.dat", "a");
	char login[60]="";
	char adress2[60]="";
	char result[400]="";

	strcpy(login, lr_eval_string("{firstName}{numberUsername}"));
	strcpy(adress2, lr_eval_string("{cityReg}/{stateReg}/{zipCodeReg}"));
		
	lr_save_string(login, "login");
	lr_save_string(adress2, "adress2");
	
	strcpy(result, lr_eval_string("{login}	{standartPassword}	{firstName}	{lastName}	{streetReg}	{adress2}	{numberCreditCard}	{expDate}\n"));
	lr_save_string(result, "result");
		
    if (fp != NULL){      
		fprintf (fp,result);
    	//fputs(result, fp);
        fclose(fp);
    } else{
        lr_error_message("Failed to open file for writing");
    }
    return;
    

}



