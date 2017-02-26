This is just a test code to check whether:-
1) You can generate Access and Refresh Token using Authorization Grant Code
2) Generate Access Token using only Refresh Token
3) Replenish the item running low using Access Token

//Enter the SSID Password of your Access Point
const char* _SSID     = "Littlt";           //Wi-Fi SSID 
const char* _PASSWORD = "Autote@111";     // Wi-Fi Password 


//AMAZON DRS parameters
const char* device_model = "OmegaJar";                //Product model ID of your device 
const char* device_identifier = "OmegaJarByESP8266";  // Serial No of device any thing you wish at production end
const char* client_id = "";                           // your Amazon developer account client ID
const char* client_secret = "";                       // Your Amazon developer account secret code 
const char* slot_id = "";                              //Sloat ID for your product
const char* redirect_uri = "https%3A%2F%2Falpha-amol.github.io"; //Encoded Return URL should be encoded and same as used at time of device creation


String authorization_grant_code = "ANXLDHgzmHRZEPokem"; //ex: ANQEgiAOjkQWjhNWIN

String refresh_Token = "Atzr|IwEBIHPFj3XoaOXNHsYhD5Hcz4mjITx9_ayIje_FGhZc148HylHRASgWizpMd8RIEYU_ZlGoTbMNTWSH1lJ1BV7XzV_hGcwZ25QtY7RQSQPXXNJ8NRvn54X2yC4jZNw-EA3LpcCpUc8Ux0r2Qus3UL67gj2q0MfJ1BgLNaDWTKD7UF3W1zMCWvfVzBEvYN-Obv5U4IThHedcRePb4g2NkFOIH-wXJIGl6hmdV5_KugiC4GzKzKnbC1i4raGapi084rJZuGfPyYCgi2kW_PEInHbKRmphQN5P1kCJqwJqP6kP0G-rWbb9h0GXzi6inQLPswKr8aDp9ZofKBe4caxl1euz7OTaXtWTjdIa2Yyl5cIporgSi1-fXHc3JEHlwU2XFn_0";

String access_Token = "";


OutPut of this Code:-
OmegaJar- Poweredby Amazon DRS

Connecting to WiFi
******************************WIFI Connecting*******************************************
....
Wi-Fi Connected!!
***************************************************************************************
IP address: 192.168.1.107
.............Access and Refresh Token.............
Connected
Sending Request for obtain Tokens
Request Send
HTTP/1.1 200 OK
Server: Server
Date: Sun, 26 Feb 2017 20:09:54 GMT
Content-Type: application/json
Content-Length: 998
Connection: keep-alive
x-amzn-RequestId: 897be67f-fc5f-11e6-bce9-e92f6050dddb
X-Amz-Date: Sun, 26 Feb 2017 20:09:54 GMT
Cache-Control: no-cache, no-store, must-revalidate
Pragma: no-cache
Vary: Accept-Encoding,User-Agent

{"access_token":"Atza|IwEBIDjrP81a6nqLNE-N6UpVMy8Kq8yTaMIKCNFBSGyPAx-vLUJCpyXqwxsIca98xD6ttAng4ycC0Yq3fSh0G33dyq7f-BHpPFC9G7eV9k13Hgkskn5W7r2tOKwqcchWZjIz-yWfUfxGvb3XHmNNCtQ-ffaze1KhHRXxhIS8rRdMfR6FxNW3nln7jkZcZc_aY2aNgJRoBhlfNRA8X6w_kOoCJc6GFY5p89EoGtIP5Q1wVk4k6lUAkTPLAQEQ1gyjX0j9JbeufOUCT3vifX2yeO6nCB1x5tTF1z6TNbybCI9x_V0ibTpcfpR_cmjW2lwQbvrzGJWMSKPnMCrqvsP-i9EVyFzokqnDm5R8MY9035m-pnIzK31dI6hmVUxFmx1L90aLgIpkdnCyVa-kZFs6V-lYmFFMt1MueKCxPh-5FHg4DxNciQAtVnOyUb8IBJjWAZEnuL8","refresh_token":"Atzr|IwEBIJhNzF8Iv6AzbFy6chfOYh281O2iA05j79CXGGIgxFPEJfiRXleyELWo_G3ssaND63cRGRgjfJ2PVag0G2XvlztGICmY0UQMWN13oZSeP-nbuZFALigmfBZgP2FBllVSJ6F1V0beQfoK-31K5GmpT-JF3lwd8dSyRb9LPbqDEmTbrwR5iTSrvUlKeWFxbuGb5btFWWijfi9JAZeVDwFkccizaq2AWX1pRgKnm1o08kMQ8ZP3oko0yWYUXXr9XCGHgXEklzNOdx9pIpAD5nSZCfS1zSdQ-US_Cj8aMHigc9e1q7P2nqowlMQcSFK9vbYscXmWD8A6nB0LvTaeup07urEPdqF05mSVPXc-li9pJNTnQh2v6xKkoO9w8GSloe_-tfQbGuBiojSihR2v8irJGnXVIdpAOPlOX9QhJkhJyv7uu5ohNXfscoKP5qlcGMSffDg","token_type":"bearer","expires_in":3555}
Access token: Atza|IwEBIDjrP81a6nqLNE-N6UpVMy8Kq8yTaMIKCNFBSGyPAx-vLUJCpyXqwxsIca98xD6ttAng4ycC0Yq3fSh0G33dyq7f-BHpPFC9G7eV9k13Hgkskn5W7r2tOKwqcchWZjIz-yWfUfxGvb3XHmNNCtQ-ffaze1KhHRXxhIS8rRdMfR6FxNW3nln7jkZcZc_aY2aNgJRoBhlfNRA8X6w_kOoCJc6GFY5p89EoGtIP5Q1wVk4k6lUAkTPLAQEQ1gyjX0j9JbeufOUCT3vifX2yeO6nCB1x5tTF1z6TNbybCI9x_V0ibTpcfpR_cmjW2lwQbvrzGJWMSKPnMCrqvsP-i9EVyFzokqnDm5R8MY9035m-pnIzK31dI6hmVUxFmx1L90aLgIpkdnCyVa-kZFs6V-lYmFFMt1MueKCxPh-5FHg4DxNciQAtVnOyUb8IBJjWAZEnuL8
Atza|IwEBIDjrP81a6nqLNE-N6UpVMy8Kq8yTaMIKCNFBSGyPAx-vLUJCpyXqwxsIca98xD6ttAng4ycC0Yq3fSh0G33dyq7f-BHpPFC9G7eV9k13Hgkskn5W7r2tOKwqcchWZjIz-yWfUfxGvb3XHmNNCtQ-ffaze1KhHRXxhIS8rRdMfR6FxNW3nln7jkZcZc_aY2aNgJRoBhlfNRA8X6w_kOoCJc6GFY5p89EoGtIP5Q1wVk4k6lUAkTPLAQEQ1gyjX0j9JbeufOUCT3vifX2yeO6nCB1x5tTF1z6TNbybCI9x_V0ibTpcfpR_cmjW2lwQbvrzGJWMSKPnMCrqvsP-i9EVyFzokqnDm5R8MY9035m-pnIzK31dI6hmVUxFmx1L90aLgIpkdnCyVa-kZFs6V-lYmFFMt1MueKCxPh-5FHg4DxNciQAtVnOyUb8IBJjWAZEnuL8
SUCCESS
Refresh token: Atzr|IwEBIJhNzF8Iv6AzbFy6chfOYh281O2iA05j79CXGGIgxFPEJfiRXleyELWo_G3ssaND63cRGRgjfJ2PVag0G2XvlztGICmY0UQMWN13oZSeP-nbuZFALigmfBZgP2FBllVSJ6F1V0beQfoK-31K5GmpT-JF3lwd8dSyRb9LPbqDEmTbrwR5iTSrvUlKeWFxbuGb5btFWWijfi9JAZeVDwFkccizaq2AWX1pRgKnm1o08kMQ8ZP3oko0yWYUXXr9XCGHgXEklzNOdx9pIpAD5nSZCfS1zSdQ-US_Cj8aMHigc9e1q7P2nqowlMQcSFK9vbYscXmWD8A6nB0LvTaeup07urEPdqF05mSVPXc-li9pJNTnQh2v6xKkoO9w8GSloe_-tfQbGuBiojSihR2v8irJGnXVIdpAOPlOX9QhJkhJyv7uu5ohNXfscoKP5qlcGMSffDg
Atzr|IwEBIJhNzF8Iv6AzbFy6chfOYh281O2iA05j79CXGGIgxFPEJfiRXleyELWo_G3ssaND63cRGRgjfJ2PVag0G2XvlztGICmY0UQMWN13oZSeP-nbuZFALigmfBZgP2FBllVSJ6F1V0beQfoK-31K5GmpT-JF3lwd8dSyRb9LPbqDEmTbrwR5iTSrvUlKeWFxbuGb5btFWWijfi9JAZeVDwFkccizaq2AWX1pRgKnm1o08kMQ8ZP3oko0yWYUXXr9XCGHgXEklzNOdx9pIpAD5nSZCfS1zSdQ-US_Cj8aMHigc9e1q7P2nqowlMQcSFK9vbYscXmWD8A6nB0LvTaeup07urEPdqF05mSVPXc-li9pJNTnQh2v6xKkoO9w8GSloe_-tfQbGuBiojSihR2v8irJGnXVIdpAOPlOX9QhJkhJyv7uu5ohNXfscoKP5qlcGMSffDg
SUCCESS
1
Connected to AP......and DRS Enabled...
.............Access and Refresh Token.............
Connected
Sending Request for obtain Tokens
Request Send
HTTP/1.1 200 OK
Server: Server
Date: Sun, 26 Feb 2017 20:10:12 GMT
Content-Type: application/json
Content-Length: 1019
Connection: keep-alive
x-amzn-RequestId: 945faf18-fc5f-11e6-9372-454c4cada2b9
X-Amz-Date: Sun, 26 Feb 2017 20:10:12 GMT
Cache-Control: no-cache, no-store, must-revalidate
Pragma: no-cache
Vary: Accept-Encoding,User-Agent

{"access_token":"Atza|IwEBIPLpC1srdEpNFISIJ5F8RbsRgjzFLL9hCxQK46Du9wD4Fgdqd2KKSKfxZ6R23Hu2t--0O6R_jmMEvMA9F_aFbwy4KlSzY6uRZiV-oEg7s8xx5Cm0oOpHZLAC0OY_YDkfQB63fgip5xQ_z-7BEsd3gABSQv8Mtg1wfnEbeEzjapZEhq6v-Q9rNz3GG3yAW-p6joC2NXcqkAuslm4bbvAVX8kDbpgp3JYoMGLUkk6C-PDBaTqZCWdDsIT7cFgLupFq4IDEeSvjPzRVDd3EabteUArd1QzdnwUkYHseXoDjEvyJMqIw0Z8AmxOV_ex0llaMu5dDncGHczR4sBDeW0LNaEyJSxFmKTvhXF5XqTgxhnP1X8jt1Pm8sEIXjcYjBHKidxxS7rgJ9Toz4PCBjcFeBeWv969jE57vsFVtfrd8B","refresh_token":"Atzr|IwEBIJhNzF8Iv6AzbFy6chfOYh281O2iA05j79CXGGIgxFPEJfiRXleyELWo_G3ssaND63cRGRgjfJ2PVag0G2XvlztGICmY0UQMWN13oZSeP-nbuZFALigmfBZgP2FBllVSJ6F1V0beQfoK-31K5GmpT-JF3lwd8dSyRb9LPbqDEmTbrwR5iTSrvUlKeWFxbuGb5btFWWijfi9JAZeVDwFkccizaq2AWX1pRgKnm1o08kMQ8ZP3oko0yWYUXXr9XCGHgXEklzNOdx9pIpAD5nSZCfS1zSdQ-US_Cj8aMHigc9e1q7P2nqowlMQcSFK9vbYscXmWD8A6nB0LvTaeup07urEPdqF05mSVPXc-li9pJNTnQh2v6xKkoO9w8GSloe_-tfQbGuBiojSihR2v8irJGnXVIdpAOPlOX9QhJkhJyv7uu5ohNXfscoKP5qlcGMSffDg","token_type":"bearer","expires_in":3600}
Access token: Atza|IwEBIPLpC1srdEpNFISIJ5F8RbsRgjzFLL9hCxQK46Du9wD4Fgdqd2KKSKfxZ6R23Hu2t-IY7Y8aT943YTZV71rlat1KZ9zporbHWs6RpNQxOohKDwNYd-0O6R_jmMEvMA9F_aFbwy4KlSzY6uRZiV-oEg7s8xx5Cm0oOpHZLAC0OY_YDkfQB63fgip5xQ_z-7BEsd3gABSQv8Mtg1wfnEbeEzjapZEhq6v-Q9rNz3GG3yAW-p6joC2NXcqkAuslm4bbvAVX8kDbpgp3JYoMGLUkk6C-PDBaTqZCWdDsIT7cFgLupFq4IDEeSvjPzRVDd3EabteUArd1QzdnwUkYHseXoDjEvyJMqIw0Z8AmxOV_ex0llaMu5dDncGHczR4sBDeW0LNaEyJSxFmKTvhXF5XqTgxhnP1X8jt1Pm8sEIXjcYjBHKidxxS7rgJ9Toz4PCBjcFeBeWv969jE57vsFVtfrd8B
Atza|IwEBIPLpC1srdEpNFISIJ5F8RbsRgjzFLL9hCxQK46Du9wD4Fgdqd2KKSKfxZ6R23Hu2t-IY7Y8aT943YTZV71rlat1KZ9zporbHWs6RpNQxOohKDwNYd-0O6R_jmMEvMA9F_aFbwy4KlSzY6uRZiV-oEg7s8xx5Cm0oOpHZLAC0OY_YDkfQB63fgip5xQ_z-7BEsd3gABSQv8Mtg1wfnEbeEzjapZEhq6v-Q9rNz3GG3yAW-p6joC2NXcqkAuslm4bbvAVX8kDbpgp3JYoMGLUkk6C-PDBaTqZCWdDsIT7cFgLupFq4IDEeSvjPzRVDd3EabteUArd1QzdnwUkYHseXoDjEvyJMqIw0Z8AmxOV_ex0llaMu5dDncGHczR4sBDeW0LNaEyJSxFmKTvhXF5XqTgxhnP1X8jt1Pm8sEIXjcYjBHKidxxS7rgJ9Toz4PCBjcFeBeWv969jE57vsFVtfrd8B
SUCCESS
1
.............REPLENISHING Jar..............
Requesting Replenishment for slotId be3e0bdf-66d1-46a0-b1a6-54fe6523bad1
Connected
Sending Request for Replenishment
Request Send
HTTP/1.1 200 OK
Server: Server
Date: Sun, 26 Feb 2017 20:10:25 GMT
Content-Type: application/json
Content-Length: 171
Connection: keep-alive
x-amzn-RequestId: 9afe528c-fc5f-11e6-9268-9d3fc78fbd44
x-amzn-type-version: com.amazon.dash.replenishment.DrsReplenishResult@1.0
Vary: Accept-Encoding,User-Agent

{"detailCode":"TEST_ORDER_PLACED","eventInstanceId":"amzn1.dash.v2.o.ci44NGY5OTMwNC0wNTcxLTQ2YWUtYjA1NC04ZmY1MTkxZTg0MzUuZDA0YjE1YzQtMzBlMy00NWYwLWIxM2UtZGE1YjFlMjBkN2I4"}
detailCode: TEST_ORDER_PLACED
SUCCESS 12 --> Test Order Placed
12
Test Order Placed..
