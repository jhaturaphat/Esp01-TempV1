
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

document.addEventListener("DOMContentLoaded", function(xhttp) {
    // ทำงานเมื่อโปรแกรมถูกโหลด    
     loadXMLDoc('/wifi.json', getConfig);
     loadXMLDoc('/config.json', configSys);
     loadXMLDoc('/influxdb.json', configInfluxdb);
     setTimeout(() => {
        loadXMLDoc('/scannetwork.json', scanNetwork);
     }, 300);
     
    });

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

   function configSys(xhttp){
    console.log(xhttp);
     var data = JSON.parse(xhttp.response);
    
    document.getElementById("ntpserver").value  = data.ntp;
    document.getElementById("ssr1low").value    = data.alarm.sensor_1.low;
    document.getElementById("ssr1high").value   = data.alarm.sensor_1.high;
    document.getElementById("ssr2low").value    = data.alarm.sensor_2.low;
    document.getElementById("ssr2high").value   = data.alarm.sensor_2.high;
   }

   function configInfluxdb(xhttp){
    console.log(xhttp);
    var data = JSON.parse(xhttp.response);
    document.getElementById("influxdb_url").value       = data.influxdb_url;
    document.getElementById("influxdb_token").value     = data.influxdb_token;
    document.getElementById("influxdb_org").value       = data.influxdb_org;
    document.getElementById("influxdb_bucket").value    = data.influxdb_bucket;
    document.getElementById("influxdb_point").value     = data.influxdb_point;
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
            text: "การตั้งค่าสำเร็จแล้ว"				  
          });	
      }
  }

  function influxdbConfig(xhttp){
    if(xhttp.status == 200){
        Swal.fire({
          icon: "success",
          title: "บันทึกสำเร็จ",
          text: "การตั้งค่าสำเร็จแล้ว"				  
        });	
    }
}

  function alarmConf(xhttp){
      if(xhttp.status == 200 )	{
          alertStyle("success", "บันทึกสำเร็จ", "การตั้งค่าสำเร็จแล้ว");					
      }
  }

  function alarmRange(xhttp){
    if(xhttp.status == 200 )	{
        alertStyle("success", "บันทึกสำเร็จ", "การตั้งค่าสำเร็จแล้ว");					
    }
  }

  function lineConf(xhttp){			
    console.log(xhttp.responseText);
      if(xhttp.status == 200 )	{
          alertStyle("success", "บันทึกสำเร็จ", "การตั้งค่าสำเร็จแล้ว");					
      }				
      
  }

  function locationConf(xhttp){			
      if(xhttp.status == 200 )	{
          alertStyle("success", "บันทึกสำเร็จ", "การตั้งค่าสำเร็จแล้ว");					
      }				
      
  }

  function apiConf(xhttp){
      if(xhttp.status == 200 )	{
          alertStyle("success", "บันทึกสำเร็จ", "การตั้งค่าสำเร็จแล้ว");					
      }				
  }

  function FormReset(){
      document.getElementsByTagName("form").reset();
  }

  function Restart(xhttp){
      if(xhttp.status === 200 ){
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

// ตั้งค่า influxdbCmd
document.getElementById("influxdbCmd").addEventListener('click', function(e){
    e.preventDefault();   

    var url = "/influxdbCfg?"
    url+="influxdb_url="+document.getElementById("influxdb_url").value;
    url+="&influxdb_token="+document.getElementById("influxdb_token").value;
    url+="&influxdb_org="+document.getElementById("influxdb_org").value;
    url+="&influxdb_bucket="+document.getElementById("influxdb_bucket").value;
    url+="&influxdb_point="+document.getElementById("influxdb_point").value;
    loadXMLDoc(url, influxdbConfig);
});
  /*ตั้งค่า Alarm */
  document.getElementById("alarmConfCmd").addEventListener('click', function(e){
      e.preventDefault();		
      let ssr1low = document.getElementById("ssr1high").value;
      let ssr1high = document.getElementById("ssr1low").value;      
      let ssr2low = document.getElementById("ssr2low").value;
      let ssr2high = document.getElementById("ssr2high").value;
      let url = "/alarmCfg?"
      +"ssr1low="+ssr1low
      +"&ssr1high="+ssr1high
      +"&ssr2low="+ssr2low
      +"&ssr2high="+ssr2high
      loadXMLDoc(url, alarmConf);
  }, false);

/*ตั้งเวลาแจ้งเตือนสถานะ*/
  document.getElementById("alarmRangeCmd").addEventListener('click',function(e){
    e.preventDefault();
    let alH1 = document.getElementById("alH1").value;
    let alH2 = document.getElementById("alH2").value;
    let alH3 = document.getElementById("alH3").value;
    let url = "/alarmRange?"
    +"alH1="+alH1
    +"&alH2="+alH2
    +"&alH3="+alH3
    loadXMLDoc(url, alarmRange);
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

