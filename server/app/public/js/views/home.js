
$(document).ready(function(){

	var hc = new HomeController();
	var av = new AccountValidator();
	
	$('#account-form').ajaxForm({
		beforeSubmit : function(formData, jqForm, options){
			if (av.validateForm() == false){
				return false;
			} 	else{
			// push the disabled username field onto the form data array //
				formData.push({name:'user', value:$('#user-tf').val()})
				return true;
			}
		},
		success	: function(responseText, status, xhr, $form){
			if (status == 'success') hc.onUpdateSuccess();
		},
		error : function(e){
			if (e.responseText == 'email-taken'){
				av.showInvalidEmail();
			}	else if (e.responseText == 'username-taken'){
				av.showInvalidUserName();
			}
		}
	});
	$('#name-tf').focus();

	//add account settings button
	var settingsButton = document.getElementById('account-settings-btn');
	settingsButton.addEventListener('click', showAccountSettings);

	var hiveRegButton = document.getElementById('reg-hive-btn');
	hiveRegButton.addEventListener('click', showRegHive);

	//hide the account settings
	var accountContainer = document.getElementById('account-form-container');
	accountContainer.style.display = 'none';

	//add dashboards button
	var dashboardButton = document.getElementById('dashboards-btn');
	dashboardButton.addEventListener('click', showDashboards);

	var dashboardsDiv = document.getElementById('dashboards');
	dashboardsDiv.style.display = 'none';

	var regHiveDiv = document.getElementById('hive-form-container');
	regHiveDiv.style.display = 'none'; 

	function showDashboards(){
		accountContainer.style.display = 'none';
		regHiveDiv.style.display = 'none';
		dashboardsDiv.style.display = '';
		var dataPoints = [{y : 10}, {y : 13}, {y : 18}, {y : 20}, {y : 17}];
		var chart = new CanvasJS.Chart("dashboards", {
				title : {
					text : "Temperature"
				},
				data : [{
						type : "spline",
						dataPoints : dataPoints
					}
				]
			});

		chart.render();
		
		var yVal = 15, updateCount = 0;
		chart.options.title.text = "Temperature";
		var updateChart = function () {

			yVal = yVal + Math.round(5 + Math.random() * (-5 - 5));
	      	updateCount++;
			
			dataPoints.push({
				y : yVal
			});
	        
			chart.render();    
			
		};

		// update chart every second
		setInterval(function(){updateChart()}, 1000);
	}

	function showAccountSettings(){
		dashboardsDiv.style.display = 'none';
		regHiveDiv.style.display = 'none';
		accountContainer.style.display = '';

		// customize the account settings form 
		$('#account-form h2').text('Account Settings');
		$('#account-form #sub1').text('Here are the current settings for your account.');
		$('#user-tf').attr('disabled', 'disabled');
		$('#account-form-btn1').html('Delete');
		$('#account-form-btn1').addClass('btn-danger');
		$('#account-form-btn2').html('Update');

		// setup the confirm window that displays when the user chooses to delete their account 
		$('.modal-confirm').modal({ show : false, keyboard : true, backdrop : true });
		$('.modal-confirm .modal-header h4').text('Delete Account');
		$('.modal-confirm .modal-body p').html('Are you sure you want to delete your account?');
		$('.modal-confirm .cancel').html('Cancel');
		$('.modal-confirm .submit').html('Delete');
		$('.modal-confirm .submit').addClass('btn-danger');
	}

	function showRegHive(){
		dashboardsDiv.style.display = 'none';
		regHiveDiv.style.display = '';
		accountContainer.style.display = 'none';

		$('#hive-form-btn').html('Add');
	}
});
