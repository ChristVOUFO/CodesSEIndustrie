var OnButton = document.getElementById("on")
var OffButton = document.getElementById("off")

function goOn(){

  OnButton.style.backgroundColor = "gray"
  OffButton.style.backgroundColor = "red" 
}

function goOff(){
  
  OffButton.style.backgroundColor = "gray"
  OnButton.style.backgroundColor = "green"
}