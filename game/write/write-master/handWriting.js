var canvasWidth = Math.min($(window).width() - 20,600);
var canvasHeight = canvasWidth;
var isMouseDown = false;
var lastLoc = {x:0,y:0};//上一次绘制坐标
var lastTimeStamp = 0;//上一次绘制时间
var lastLineWidth = -1;//上次线宽
var strokeColor = '#000';
var maxLineWidth = 30;
var minLineWidth = 1;
var maxStrokeV = 10;
var minStrokeV = 0.1;
var resultLineWidth;
var canvas = document.getElementById('canvas');
var cxt = canvas.getContext('2d');

canvas.width = canvasWidth;
canvas.height = canvasHeight;
$('#controller').css('width',canvasWidth);

drawGrid();//绘制米字格


canvas.onmousedown = function(e){
	e.preventDefault();
	startStroke({x:e.clientX,y:e.clientY});
}
canvas.onmousemove = function(e){
	e.preventDefault();
	strokeMove({x:e.clientX,y:e.clientY});//鼠标移动开始绘制路线
}
canvas.onmouseout = function(e){
	e.preventDefault();
	endStorke();
}

canvas.onmouseup= function(e){
	e.preventDefault();
	endStorke();
}
//适配手机端，添加触碰事件
canvas.addEventListener('touchstart',function(e){
	e.preventDefault()
	touch = e.touches[0]
	startStroke( {x: touch.clientX , y: touch.clientY} )
});
canvas.addEventListener('touchmove',function(e){
	e.preventDefault()

	touch = e.touches[0]
	strokeMove({x: touch.clientX , y: touch.clientY})

});
canvas.addEventListener('touchend',function(e){
	e.preventDefault()
	endStorke()
});

//改变颜色和清除控件
$('.color_btn').click(
	function(e){
		$('.color_btn').removeClass('color_btn_selected');
		$(this).addClass('color_btn_selected');
		strokeColor = $(this).css('backgroundColor');	
	})
$('#clear_btn').click(
	function() {
		cxt.clearRect(0,0,canvasWidth,canvasHeight);
		drawGrid();
	})

//把window坐标转变为canvas坐标
function WindowToCanvas(x,y){
	var bbox = canvas.getBoundingClientRect();
	return {x:x - bbox.left,y:y - bbox.top}
}
function startStroke (point) {
	isMouseDown = true;
	lastLoc = WindowToCanvas(point.x,point.y);
	lastTimeStamp = new Date().getTime();
}

function strokeMove(point){
	if (isMouseDown) {
		var curLoc = WindowToCanvas(point.x,point.y);
		var curTimeStamp = new Date().getTime();
		var t = curTimeStamp - lastTimeStamp;
		var s = calcDistance(curLoc,lastLoc);
		var lineWidth = calcLineWidth(s,t);

 		//draw
 		cxt.beginPath();
 		cxt.moveTo(lastLoc.x,lastLoc.y);
 		cxt.lineTo(curLoc.x,curLoc.y);
 		cxt.closePath();
 		cxt.strokeStyle = strokeColor;
 		cxt.lineWidth = lineWidth;
 		cxt.lineCap = 'round';
 		cxt.lineJoin = 'round';
 		cxt.stroke();
 		lastLoc = curLoc;
 		lastTimeStamp = curTimeStamp;
 		lastLineWidth = lineWidth;
 	}
}
function endStorke () {
	isMouseDown = false;
}

//根据鼠标移动速度改变笔画粗细
function calcLineWidth(s,t){
	var v = s/t;
	if (v<minStrokeV) {
		resultLineWidth = maxLineWidth;
	}
	else if (v>maxStrokeV) {
		resultLineWidth = minLineWidth;
	}
	else{
		resultLineWidth = maxLineWidth - (v - minStrokeV)/(maxStrokeV - minStrokeV)*(maxLineWidth - minLineWidth);//根据速度和最大线条宽度占比计算显示的线条宽度
	}
	if (lastLineWidth ==-1){
		return resultLineWidth;
	}
	return resultLineWidth*2/4 + lastLineWidth*3/6;//根据线条宽度与上次线条宽度比例计算显示线条宽度
}
//计算两点间距离
function calcDistance(loc1,loc2){
	return Math.sqrt(Math.pow(loc1.x - loc2.x,2)+Math.pow(loc1.y - loc2.y,2));
}

function drawGrid(){
	cxt.save();
	cxt.strokeStyle = '#CC0000';

	cxt.beginPath();
	cxt.moveTo(3,3);
	cxt.lineTo(canvasWidth - 3,3);
	cxt.lineTo(canvasWidth - 3,canvasHeight - 3);
	cxt.lineTo(3,canvasHeight - 3);
	cxt.closePath();

	cxt.lineWidth = 6;
	cxt.stroke();

	cxt.beginPath();
	cxt.moveTo(0,0);
	cxt.lineTo(canvasWidth,canvasHeight);

	cxt.moveTo(canvasWidth,0);
	cxt.lineTo(0,canvasHeight);

	cxt.moveTo(canvasWidth / 2,0);
	cxt.lineTo(canvasWidth / 2,canvasHeight);

	cxt.moveTo(0,canvasHeight / 2);
	cxt.lineTo(canvasWidth,canvasHeight / 2);
	cxt.closePath();

	cxt.lineWidth = 1;
	cxt.setLineDash([4,3]);//虚线
	cxt.stroke();

	cxt.restore();
}
