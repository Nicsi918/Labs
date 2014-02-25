function SignUp(){
		var fname = document.getElementById("fname").value;
		var sname = document.getElementById("sname").value;
		var city = document.getElementById("city").value;
		var country = document.getElementById("country").value;
		var email = document.getElementById("emailSignup").value;
		var pass1 = document.getElementById("passwordSignup1").value;
		var pass2 = document.getElementById("passwordSignup2").value;
		
		if (fname == "") 
			document.getElementById("fname").style.border="solid 1px red";
		if( sname == "")
			document.getElementById("sname").style.border="solid 1px red";
		if( city == "")
			document.getElementById("city").style.border="solid 1px red";
		if( country == "")
			document.getElementById("country").style.border="solid 1px red";
		if( email == "" || email.indexOf('@') == -1)
			document.getElementById("emailSignup").style.border="solid 1px red";
		if( pass1 == "" || pass1 != pass2)
		{
			document.getElementById("passwordSignup1").style.border="solid 1px red";
			document.getElementById("passwordSignup2").style.border="solid 1px red";
			document.getElementById("passwordSignup1").value = "";
			document.getElementById("passwordSignup2").value = "";
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
			var result = serverstub.signUp(data);


			console.log(result);
			document.getElementById("signUpStatus").innerHTML = result.message;
			if (!result.success && result.message == "User already exists.")
				document.getElementById("emailSignup").style.border="solid 1px red";
			else if(result.success)
				document.getElementById("signupForm").reset();				
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
	var username = document.getElementById("email").value;
	var password = document.getElementById("psw").value;
	
	var ret = serverstub.signIn(username, password);
	
	document.getElementById("signInStatus").innerHTML = ret.message;

	//document.getElementById("login-form").action = "{{ url_for('/add_entry') }}";
	document.getElementById("login-form").action = "/add_entry";
	document.getElementById("login-form").submit();

	if(ret.success){
		document.getElementById("main-container").innerHTML = document.getElementById("profileView").innerHTML;
		loadHomeWall();
		loadHomePersonalInfo();
		sessionStorage.useremail = null;
		sessionStorage.token = ret.data;
		console.log(sessionStorage.token);
	}
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
	if (newPw == document.getElementById("newPwr2").value && newPw!="")
		ret = serverstub.changePassword(sessionStorage.token, oldPw, newPw);
	else{
		document.getElementById("newPwr1").style.border="solid 1px red";
		document.getElementById("newPwr2").style.border="solid 1px red";
	}	
	if (ret.success)
		document.getElementById("AccountForm").reset();
			
	document.getElementById("changeStatus").innerHTML = ret.message;
};
function SignOut(){
	var ret = serverstub.signOut(sessionStorage.token);
	if (ret.success)
		document.getElementById("main-container").innerHTML = document.getElementById("welcomeView").innerHTML;
};

function postOnOwnWall(){
	var message = document.getElementById("postMessageTextbox-home").value;	
	var retData = serverstub.getUserDataByToken(sessionStorage.token);
	var mail = retData.data.email;
	postMessage(message, mail, "-home");
};

function postOnOtherWall(){
	var message = document.getElementById("postMessageTextbox-browse").value;	
	var retData = serverstub.getUserDataByToken(sessionStorage.token);
	var mail = sessionStorage.useremail;
	postMessage(message, mail, "-browse");
};

function postMessage(message, mail, id) {
	var ret = serverstub.postMessage(sessionStorage.token, message, mail);
	if (ret.success){
		document.getElementById("postMessageTextbox"+ id).value = "";
		if (id == "-home") loadHome();
		if (id == "-browse") loadBrowse();
	}else{
		
	}
};

function loadHome(){
	loadHomeWall();
	loadHomePersonalInfo();
};

function loadHomeWall(){
	var retData = serverstub.getUserDataByToken(sessionStorage.token);
	var email = retData.data.email;	
 	loadMessagesToWall(email, "-home");
};

function loadMessagesToWall(email, id){
	var ret = serverstub.getUserMessagesByEmail(sessionStorage.token, email);
	//console.log(ret);
	var htmlFinal = "";
	
	for (var i = 0; i < ret.data.length; i++){
		var writerMail = ret.data[i].writer;
		var retWriterData = serverstub.getUserDataByEmail(sessionStorage.token, writerMail);
		//console.log(retWriterData);
		var writerFirstName = retWriterData.data.firstname;
		var writerFamilyName = retWriterData.data.familyname;
		var content = ret.data[i].content;	
		
		var htmlString = "<p class='messageClass'>"
		htmlString = htmlString + content + '<p class="timestamp">' + writerFirstName + " " + writerFamilyName + "</p></p>";
		htmlFinal = htmlFinal + htmlString;
	}
	
	document.getElementById("messageWall" + id).innerHTML = htmlFinal;
	
	if (ret.success){
		
	}
	else{
		
	}
	
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
	var userData = serverstub.getUserDataByEmail(sessionStorage.token, email);
		console.log(userData);

	if (userData.success){
		loadPersonalInfo(email, "-browse");
		loadMessagesToWall(email, "-browse");
		sessionStorage.useremail = email;
	}else{
		
	}
};

function loadHomePersonalInfo(){
	var retData = serverstub.getUserDataByToken(sessionStorage.token);
	console.log(retData);
	var email = retData.data.email;	
 	loadPersonalInfo(email, "-home");
};

function loadPersonalInfo(email, id){
	var userData = serverstub.getUserDataByEmail(sessionStorage.token, email);
	var userInfoString = userData.data.firstname + "<br>" + userData.data.familyname + "<br>" + userData.data.email;
	userInfoString = userInfoString + "<br>" + userData.data.city + "<br>" + userData.data.country + "<br>" + userData.data.gender;
	console.log(userData);
	console.log(userInfoString);
	document.getElementById("userInfoLabel" + id).innerHTML = userInfoString;
	document.getElementById("userInfoLabel" + id).style.fontSize = 4;
	
};
	