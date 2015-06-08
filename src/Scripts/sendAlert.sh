#/bin/sh
# LifeCatch Send SMS Script

source /mnt/sda1/LifeCatch/lifecatch.conf
source /mnt/sda1/LifeCatch/userdata.conf

fall_str="fall"
button_str="button"
if [ $1 = $fall_str ]
then
	message="$fallmessage"
	curl -d "user=$username&alert_type=Fall_Detection_Alert" http://georgebraxton.com/lifecatch/add_alert.php
elif [ $1 = $button_str ]
then
	message="$buttonmessage"
	curl -d "user=$username&alert_type=Manual_Alert" http://georgebraxton.com/lifecatch/add_alert.php
else
	exit 1
fi

sms_str="SMS"
email_str="Email"
if [ $econtactmethod = $sms_str ]
then
	temboo -a $TEMBOO_ACCOUNT -u $TEMBOO_APP_KEY_NAME -p $TEMBOO_APP_KEY -c /Library/Twilio/SMSMessages/SendSMS -i AccountSID:$TWILIO_ACCOUNT_SID -i AuthToken:$TWILIO_AUTH_TOKEN -i From:$TWILIO_NUMBER -i To:$ecsmsnum -i Body:"$message"
elif [ $econtactmethod = $email_str ]
then
	temboo -a $TEMBOO_ACCOUNT -u $TEMBOO_APP_KEY_NAME -p $TEMBOO_APP_KEY -c /Library/Google/Gmail/SendEmail -i Username:$GMAIL_USERNAME -i Password:$GMAIL_PASSWORD -i ToAddress:$ecemail -i Subject:"$GMAIL_SUBJECT" -i MessageBody:"$message"
else
	exit 2
fi


