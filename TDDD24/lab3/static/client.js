



function createXMLHTTPObject(){
	if (window.XMLHttpRequest)
  	{// code for IE7+, Firefox, Chrome, Opera, Safari
  		return new XMLHttpRequest();
  	}
else
  {// code for IE6, IE5
  	return new ActiveXObject("Microsoft.XMLHTTP");
  }
}




var socket;
function initSocket(){

	socket = new WebSocket("ws://localhost:5000/connectWS");

	socket.onmessage = function(event) {
		//console.log(event);
		console.log("Message Received");
		var msg = JSON.parse(event.data).message;
		if (msg == "Update"){
			loadHomeWall();
			loadBrowse();
		}
	}

	socket.onopen = function(event) {
		console.log("Connected");
		if (socket.readyState == 1) {
			console.log(JSON.stringify({token: sessionStorage.token}));
			socket.send(JSON.stringify({token: sessionStorage.token}));
		}
	}

	socket.onerror = function(event) {
		console.log("Error" + event.code + event.reason); 
	}

	socket.onclose = function (event) {            
	    console.log("Closed connection" + event.code + event.reason + event.wasClean);  
	};
}

function SignUp(){
		var fname = document.getElementById("fname").value;
		var sname = document.getElementById("sname").value;
		var city = document.getElementById("city").value;
		var country = document.getElementById("country").value;
		var email = document.getElementById("emailSignup").value;
		var pass1 = document.getElementById("passwordSignup1").value;
		var pass2 = document.getElementById("passwordSignup2").value;
		
		if (fname == ""){
			document.getElementById("signUpStatus").innerHTML = "Incorrect input";
			document.getElementById("fname").style.border="solid 1px red";}
		if( sname == ""){
			document.getElementById("signUpStatus").innerHTML = "Incorrect input";
			document.getElementById("sname").style.border="solid 1px red";}
		if( city == ""){
			document.getElementById("signUpStatus").innerHTML = "Incorrect input";
			document.getElementById("city").style.border="solid 1px red";}
		if( country == ""){
			document.getElementById("signUpStatus").innerHTML = "Incorrect input";
			document.getElementById("country").style.border="solid 1px red";}
		if( email == "" || email.indexOf('@') == -1){
			document.getElementById("signUpStatus").innerHTML = "Incorrect input";
			document.getElementById("emailSignup").style.border="solid 1px red";}
		if( pass1 == "" || pass1 != pass2)
		{
			document.getElementById("passwordSignup1").style.border="solid 1px red";
			document.getElementById("passwordSignup2").style.border="solid 1px red";
			document.getElementById("passwordSignup1").value = "";
			document.getElementById("passwordSignup2").value = "";
			document.getElementById("signUpStatus").innerHTML = "Incorrect input";
		}
		if (fname != "" && sname != "" && city != "" && country != "" && email != "" && pass1 != "" && pass1 == pass2 && email.indexOf('@') != -1)
		{			
			var data = new Object();
			data.email = email;
			data.password = pass1;
			data.familyname = sname;
			data.firstname = fname;
			data.city = city;
			data.country = country;
			data.gender = document.getElementById("gen").value;
			var xmlhttp = createXMLHTTPObject();
			xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				document.getElementById("signUpStatus").innerHTML = result.message;
					if (!result.success && result.message == "User already exists.")
						document.getElementById("emailSignup").style.border="solid 1px red";
					else if(result.success)
						document.getElementById("signupForm").reset();
    			}
  			}
  			xmlhttp.open("POST","/signup",true);
  			xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
			xmlhttp.send("firstname=" + fname + "&familyname=" + sname + "&gender=" + data.gender + "&email=" + email + "&city=" + city + "&country=" + country + "&password=" + pass1);	
		}
	};
	
function resetBorder(id){
	if (document.getElementById(id).style.border)
			document.getElementById(id).value = "";	

	document.getElementById(id).style.border="";	
}

function resetSignupField(id){
	resetBorder(id);
	document.getElementById("signUpStatus").innerHTML = "";
	if (id == "passwordSignup1" || id == "passwordSignup2")
	{
		document.getElementById("passwordSignup1").style.border="";
		document.getElementById("passwordSignup2").style.border="";
	}
};
function signIn(){
	//initSocket();
	var username = document.getElementById("email").value;
	var password = document.getElementById("psw").value;
	//var ret = serverstub.signIn(username, password);

	var xmlhttp = createXMLHTTPObject();
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				document.getElementById("signInStatus").innerHTML = result.message;
					if(result.success){
						sessionStorage.token = result.token;
						document.getElementById("main-container").innerHTML = document.getElementById("profileView").innerHTML;
						loadHomeWall();
						loadHomePersonalInfo();

						if ('WebSocket' in window){
						   /* WebSocket is supported. You can proceed with your code*/
						   initSocket();
						   console.log("works");
						} else {
							console.log("Doesnt work");
							   /*WebSockets are not supported. Try a fallback method like long-polling etc*/
						}
					}
					else{
						document.getElementById("email").style.border="solid 1px red";
						document.getElementById("psw").style.border="solid 1px red";
					}
    			}
  			}
  	xmlhttp.open("POST","/signin",true);
  	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send("email=" + username + "&password=" + password);
};

function showTab(id, li_id){
	document.getElementsByClassName("tabContent").item(0).style.display="none";
	document.getElementsByClassName("tabContent").item(1).style.display="none";
	document.getElementsByClassName("tabContent").item(2).style.display="none";

	
	document.getElementById("tabHome").className = "";
	document.getElementById("tabBrowse").className = "";
	document.getElementById("tabAccount").className = "";	

	document.getElementById(li_id).className = "selected";
	document.getElementById(id).style.display = "block";
};
function changePassword(){
	var oldPw = document.getElementById("oldPwr").value;
	var newPw = document.getElementById("newPwr1").value;
	if (newPw == document.getElementById("newPwr2").value && newPw!=""){
		//ret = serverstub.changePassword(sessionStorage.token, oldPw, newPw);
		var xmlhttp = createXMLHTTPObject();
		xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);

    				if (result.success)
						document.getElementById("AccountForm").reset();
			
					document.getElementById("changeStatus").innerHTML = result.message;
    			}
  			}
		xmlhttp.open("POST","/changePassword",true);
		xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
		xmlhttp.send("oldPassword=" + oldPw + "&newPassword=" + newPw + "&token=" + sessionStorage.token);
	}else{
		document.getElementById("newPwr1").style.border="solid 1px red";
		document.getElementById("newPwr2").style.border="solid 1px red";
	}	
	
};
function SignOut(){

	socket.send("WADJWJD");
	console.log("FWT?F");
	//var ret = serverstub.signOut(sessionStorage.token);

	var xmlhttp = createXMLHTTPObject();
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				if (result.success)
						document.getElementById("main-container").innerHTML = document.getElementById("welcomeView").innerHTML;
    			}
  			}
	xmlhttp.open("POST","/signout",true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send("token=" + sessionStorage.token);
	
};

function postOnOwnWall(){
	var message = document.getElementById("postMessageTextbox-home").value;	

	var xmlhttp = createXMLHTTPObject();
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				var mail = result.data.email;
					postMessage(message, mail, "-home");
    			}
  			}
	xmlhttp.open("GET","/getUserDataByToken?token=" + sessionStorage.token,true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
};

function postOnOtherWall(){
	var message = document.getElementById("postMessageTextbox-browse").value;	
	var email = sessionStorage.useremail;

	/*var xmlhttp = createXMLHTTPObject();
	var retData = serverstub.getUserDataByToken(sessionStorage.token);
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				var mail = result.data.email;
					postMessage(message, email, "-browse");
    			}
  			}
	xmlhttp.open("GET","/getUserDataByToken?token=" + sessionStorage.token,true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();*/

	postMessage(message, email, "-browse");
};

function postMessage(message, mail, id) {

	socket = new WebSocket("ws://localhost:5000/socket");

	var xmlhttp = createXMLHTTPObject();
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				if (result.success){
						document.getElementById("postMessageTextbox"+ id).value = "";
						//if (id == "-home") loadHome();
						//if (id == "-browse") loadBrowse();
						//console.log(id);
					}else{
						
					}
    			}
  			}
	xmlhttp.open("POST","/postMessage",true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send("email=" + mail + "&message=" + message + "&token=" + sessionStorage.token);
};

function loadHome(){
	loadHomeWall();
	loadHomePersonalInfo();
};

function loadHomeWall(){
	var xmlhttp = createXMLHTTPObject();
 	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				var email = result.data.email;	
 					loadMessagesToWall(email, "-home");
    			}
  			}
  	//console.log("Get messages home");
	xmlhttp.open("GET","/getUserDataByToken?token=" + sessionStorage.token,true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
};

function loadMessagesToWall(email, id){

	//var retWriterData = serverstub.getUserDataByEmail(sessionStorage.token, writerMail);   //////FIFISAOIFNASKJNF FIFIFIIFIFA
	var xmlhttp = createXMLHTTPObject();
	//console.log("should ve getting mesages");
		xmlhttp.onreadystatechange=function()
			{
				if (xmlhttp.readyState==4 && xmlhttp.status==200)
				{
					rVal = xmlhttp.responseText;
					var result = JSON.parse(rVal);
					//console.log(result);
					var htmlFinal = "";

					for (var i = result.data.length - 1; i >= 0; i--){

						var writerMail = result.data[i].fromEmail;

						rVal = xmlhttp.responseText;
						var writerData = JSON.parse(rVal);
						//console.log(retWriterData);
						var content = result.data[i].message;	
						
						var htmlString = "<p class='messageClass'>"
						htmlString = htmlString + content + '<p class="timestamp">' + writerMail + "</p></p>";
						htmlFinal = htmlFinal + htmlString;
					}				
					document.getElementById("messageWall" + id).innerHTML = htmlFinal;				
				}
			}
			xmlhttp.open("GET","/getUserMessagesByEmail?email=" + email + "&token=" + sessionStorage.token,true);
			xmlhttp.send();

	
};

function searchUser(){
	var userEmail = document.getElementById("SearchBar").value;
	updateBrowse(userEmail);
};

function loadBrowse(){
	if (sessionStorage.useremail != null){
		updateBrowse(sessionStorage.useremail);
	};
};

function updateBrowse(email){
	//var userData = serverstub.getUserDataByEmail(sessionStorage.token, email);
	//	console.log(userData);
	var xmlhttp = createXMLHTTPObject();
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				if (result.success){
						loadPersonalInfo(email, "-browse");
						loadMessagesToWall(email, "-browse");
						sessionStorage.useremail = email;
					}else{
						
					}
    			}
  			}
	xmlhttp.open("GET","/getUserDataByEmail?token=" + sessionStorage.token + "&email=" + email,true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();	
};

function loadHomePersonalInfo(){
	//var retData = serverstub.getUserDataByToken(sessionStorage.token);
	//console.log(retData);
	var xmlhttp = createXMLHTTPObject();
 	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				//console.log(result);
    				var email = result.data.email;	
 					loadPersonalInfo(email, "-home");
    			}
  			}
	xmlhttp.open("GET","/getUserDataByToken?token=" + sessionStorage.token,true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();
};

function loadPersonalInfo(email, id){
	//var userData = serverstub.getUserDataByEmail(sessionStorage.token, email);	
	var xmlhttp = createXMLHTTPObject();
	xmlhttp.onreadystatechange=function()
  			{
  				if (xmlhttp.readyState==4 && xmlhttp.status==200)
    			{
    				rVal = xmlhttp.responseText;
    				var result = JSON.parse(rVal);
    				//console.log(result);
    				var userInfoString = result.data.firstname + "<br>" + result.data.familyname + "<br>" + result.data.email;
					userInfoString = userInfoString + "<br>" + result.data.city + "<br>" + result.data.country + "<br>" + result.data.gender;
					//console.log(result);
					//console.log(userInfoString);
					document.getElementById("userInfoLabel" + id).innerHTML = userInfoString;
					document.getElementById("userInfoLabel" + id).style.fontSize = 4;
    			}
  			}
	xmlhttp.open("GET","/getUserDataByEmail?token=" + sessionStorage.token + "&email=" + email,true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send();	
	
};
	
