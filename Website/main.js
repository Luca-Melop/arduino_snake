//Input converter (Number to LED's on Matrix)
var dict_one = { //Dictionary --> to convert input number to array (for LED's)
    0: one_zero,
    1: one_one,
    2: one_two,
    3: one_three,
    4: one_four,
    5: one_five,
    6: one_six,
    7: one_seven,
    8: one_eight,
    9: one_nine,
};

var dict_two = {
    0: two_zero,
    1: two_one,
    2: two_two,
    3: two_three,
    4: two_four,
    5: two_five,
    6: two_six,
    7: two_seven,
    8: two_eight,
    9: two_nine,
};

var dict_three = {
    0: three_zero,
    1: three_one,
    2: three_two,
    3: three_three,
    4: three_four,
    5: three_five,
    6: three_six,
    7: three_seven,
    8: three_eight,
    9: three_nine,
};


//To get the push request from esp8266 via grandeur
const apiKey = "grandeurlaa8tsgp004f0joh1q9oalgz";
const accessKey = "18378ec16aac1f3412fa4515a1f22bfaafd73a2035c989f4dbce67adc3e65f3f";
const accessToken = "9712b242c35ecf6cfe31963bc3081e9cd9b05b72c74ae464d216fbddd843086a";

// This connects the webpage to the internet.
const project = grandeur.init(apiKey, accessKey, accessToken);
project.auth().login("lucamezger88@gmail.com", "QJZvKp4riip8nUcgouDyRphCiw4ZDyBqN");




function input() {
	// This subscribes to the "millis" variable.
	project.devices().device("devicelaa8tszv004i0joh6vyq3iw0").data().on("millis", (path, value) => document.getElementById("data").innerHTML = value);	
	console.log(document.getElementById("data").innerHTML);
	var highscore_data = document.getElementById("data").innerHTML;
	console.log("function called");
	

    //reset the LED's so that it's possible to submit multiple numbers on one session
    for (var i = 0; i < 6; i++) {
        for (e of document.getElementsByClassName("on")) {
            e.className = "off";
        }
    }

	
    var input = highscore_data; //input stored as a string
	console.log(input)
	

    const len = input.toString().length; //storing length to calculate the correct position of number (if number has less than 2 digits)
    var input1 = input[len - 1]; //digit left
    var input2 = input.charAt(len - 2); //digit middle
    var input3 = input.charAt(len - 3); //digit right

    //Digits Variables
    let digit_one = dict_one[Number(input1)]; //add the arrays for the digits via the Dictionary
    let digit_two = dict_two[Number(input2)];
    let digit_three = dict_three[Number(input3)];


    //Loop to display LED's
    const all_digits = digit_one.concat(digit_two, digit_three)
    console.log(all_digits)
    for (s of all_digits) //loops trough digit and sets every ID to "on"
        document.getElementById(s).className = "on";


    //deleting first 0's --> so that it's not  e.g. 064 but only 64 without the 0
    if (len == 2){
        for (s of three_block) //loops trough whole block and sets it to "off"
        document.getElementById(s).className = "off";
    }
    else if (len == 1){ 
        for (s of three_block.concat(two_block)) //loops trough whole block and sets it to "off"
        document.getElementById(s).className = "off";
    }
}

//call function a given times
input();


