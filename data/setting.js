


document.addEventListener("DOMContentLoaded", function(xhttp) {
    // ทำงานเมื่อโปรแกรมถูกโหลด
     loadXMLDoc('/wifi.json', getConfig);
     setTimeout(() => {
        loadXMLDoc('/scannetwork.json', scanNetwork);
     }, 300);
     
    });

  function loadXMLDoc(url,cFunction) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {				      
            cFunction(this);
      }else if (this.status == 400){
          alertStyle("error", "Oops...", text= this.responseText);			    	
      }else if (this.status >= 401) {
          alertStyle("error", "Oops...", text= this.responseText);	
      }
      };
        xhttp.open("GET", url, true);
        xhttp.send();
  }

  function alertStyle(icon = "error", title = "Oops...", text = "เกิดข้อผิดพลาดบางอย่าง"){
      Swal.fire({
        icon: icon,
        title: title,
        text: text				  
      });
  }

  function ValidateIPaddress(inputText)
   {
   var ipformat = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
       if(inputText.match(ipformat)){			 
       return true;
   }else{				 
       return false;
       }
   }
   
   

   function getConfig(xhttp){
       //console.log(xhttp.response);
       var obj = JSON.parse(xhttp.response);
       if(obj.status === "ALLOW"){
           document.getElementById('ssid').value = obj.NETWORK[0];
           document.getElementById('Password').value = obj.NETWORK[1];
           document.getElementById('IP').value = obj.NETWORK[2];
           document.getElementById('GW').value = obj.NETWORK[3];

           document.getElementById('TOKEN').value = obj.LINETOKEN;
           document.getElementById('LOCATION').value = obj.location;
           document.getElementById('URL').value = obj.urlApi;
          if(document.getElementById("DEVICE1").checked){
              document.getElementById('LOW').value = obj.alarmT1[0];
              document.getElementById('HIGH').value = obj.alarmT1[1];
          }else if(document.getElementById("DEVICE2").checked){
              document.getElementById('LOW').value = obj.alarmT2[0];
              document.getElementById('HIGH').value = obj.alarmT2[1];
          }
          document.getElementById('reboot').style.display = '';
       }else{
          document.getElementById('reboot').style.display = 'none';
           alertStyle('error','Oops...',obj.status);
       }
   }

 function scanNetwork(xhttp){
    console.log(xhttp);
    var ssids = JSON.parse(xhttp.response);
    ssids.sort((a, b) => b.rssi - a.rssi);
    var ssidArray = ssids.map(obj => obj.ssid);
    console.log(ssidArray);
    autocomplete(document.getElementById("ssid"), ssidArray);
 }

  function networkconfig(xhttp){
      if(xhttp.status == 200)	{
          Swal.fire({
            icon: "success",
            title: "บันทึกสำเร็จ",
            text: "การั้งค่าสำเร็จแล้ว"				  
          });				
      }else if(xhttp.status > 200){
        alertStyle('error','Oops...');
      }
  }

  function ntpConfig(xhttp){
      if(xhttp.status == 200){
          Swal.fire({
            icon: "success",
            title: "บันทึกสำเร็จ",
            text: "การั้งค่าสำเร็จแล้ว"				  
          });	
      }
  }

  function alarmConf(xhttp){
      if(xhttp.status == 200 && xhttp.responseText === "success")	{
          alertStyle("success", "บันทึกสำเร็จ", "การั้งค่าสำเร็จแล้ว");					
      }
  }

  function lineConf(xhttp){			
      if(xhttp.status == 200 && xhttp.responseText === "success")	{
          alertStyle("success", "บันทึกสำเร็จ", "การั้งค่าสำเร็จแล้ว");					
      }				
      
  }

  function locationConf(xhttp){			
      if(xhttp.status == 200 && xhttp.responseText === "success")	{
          alertStyle("success", "บันทึกสำเร็จ", "การั้งค่าสำเร็จแล้ว");					
      }				
      
  }

  function apiConf(xhttp){
      if(xhttp.status == 200 && xhttp.responseText === "success")	{
          alertStyle("success", "บันทึกสำเร็จ", "การั้งค่าสำเร็จแล้ว");					
      }				
  }

  function FormReset(){
      document.getElementsByTagName("form").reset();
  }

  function Restart(xhttp){
      if(xhttp.status === 200 && xhttp.responseText === "success"){
          alertStyle("success", "กำลังรีสตาร์จ", "การรีสตาร์จจะทำในไม่กีวินาที");
      }
  }

  
  /*ตั้งค่า networkconfig */
  document.getElementById("networkconfigCmd").addEventListener('click', function(e){ 
      e.preventDefault();		
      if(!ValidateIPaddress(document.getElementById("IP").value)){
          alertStyle("error", "Oops...","ตรวจสอบ IP Address ใหม่");	
          return;
      }	
     
      // ดึงข้อมูลจากฟอร์ม HTML
    var ssid = document.getElementById('ssid').value;
    var password = document.getElementById('Password').value;
    var wifichk = document.getElementById('wifichk').value;
    var fixip = document.getElementById('fixip').checked;
    var ipaddress = document.getElementById('IP').value;
    var subnetmask = document.getElementById('SN').value;
    var gateway = document.getElementById('GW').value;
    var dns = document.getElementById('DNS').value;

    var url = "/ntwcfg?"
    +"ssid="+ssid
    +"&password="+password
    +"&wifichk="+wifichk
    +"&fixip="+fixip
    +"&ipaddress="+ipaddress
    +"&subnetmask="+subnetmask
    +"&gateway="+gateway
    +"&dns="+dns
      loadXMLDoc(url, networkconfig);
     
  }, false);

  document.getElementById("fixip").addEventListener('change',function(e){
    const element = document.getElementsByClassName("network");
    if (e.currentTarget.checked) {        
        // alert('checked');        
        for(i=0; i<element.length; i++){
            element[i].style.display = "flex";
        }
      } else {
        for(i=0; i<element.length; i++){
            element[i].style.display = "none";
        }
      }
  }), false;

  document.getElementById("wlanReset").addEventListener('click', function(e){
    e.preventDefault();
    document.getElementById("fixip").checked = false;
    const element = document.getElementsByClassName("network");
    for(i=0; i<element.length; i++){
        element[i].style.display = "none";
    }
  })

  /* ตั้งค่า ntpConfigCmd */	
  document.getElementById("ntpConfigCmd").addEventListener('click', function(e){
      e.preventDefault();
      var url = "/ntpCfg?ntpserver="+document.getElementById("ntpserver").value;
      loadXMLDoc(url, ntpConfig);
  }, false);	

  
  /* ดูการตั้งค่า Alarm Sensors */
  document.getElementById("DEVICE1").addEventListener('click', function(e){
      loadXMLDoc('/getconfig', getConfig);
  });
  document.getElementById("DEVICE2").addEventListener('click', function(e){
      loadXMLDoc('/getconfig', getConfig);
  });

  /*ตั้งค่า Alarm */
  document.getElementById("alarmConfCmd").addEventListener('click', function(e){
      e.preventDefault();		
      
      loadXMLDoc(url, alarmConf);
  }, false);

  /*ตั้งค่า line token */
  document.getElementById("lineConfCmd").addEventListener('click', function(e){
      e.preventDefault();				
      var url = "/lineConf?TOKEN="+document.getElementById("TOKEN").value;
      loadXMLDoc(url, lineConf);
  }, false);

  /*ตั้งค่า สถานที่ติดตั้ง */
  document.getElementById("locationConfCmd").addEventListener('click', function(e){
      e.preventDefault();				
      var url = "/locationConf?LOCATION="+document.getElementById("LOCATION").value;
      loadXMLDoc(url, locationConf);
  }, false);

  /*ตั้งค่าที่อยู่เครื่อง api server ที่ต้องการเก็บ log*/
  document.getElementById("apiConfCmd").addEventListener('click', function(e){
      e.preventDefault();				
      var url = "/urlconfigApi?URL="+document.getElementById("URL").value;
      loadXMLDoc(url, apiConf);
  }, false);

  // รีสตาร์จเครื่อง
  document.getElementById("apiRestrt").addEventListener('click', function(e){
      e.preventDefault();
      var url = "/restart";
      loadXMLDoc(url,Restart);
  }, false);

