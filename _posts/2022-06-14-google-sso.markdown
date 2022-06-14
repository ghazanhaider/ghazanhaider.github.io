---
layout: post
title:  "Google SSO"
date:   2022-06-14 00:00:00 -0400
---
## What is Oauth2

Oauth2 is a mechanism for authentication and authorization for third party apps.

In a nutshell, there is an identity provider that knows who you are (Google). You use passwords, MFA etc to login to this identity provider.

And then there's the third party app that also needs to know who you are, and possibly anything additional about your identity that you want to share. This app needs to know Joe Smith is indeed Joe Smith (without having to maintain a separate password database). The only other way it can do that is to check with the identity provider (Google).

Now Google will not willy nilly confirm your identity to just anyone external that asks. So this is a 3-way trust between the identity provider, 3rd party app, and the person logging in. This is called **Authentication** and sometimes shortenened to **authn**.

The 3rd party app might also need more than just an identity check. It might need your first name, picture thumbnail, email address or access to your Gmail account. The developer setting up the trust between Google and this app gets to decide the set of permissions that Google can allow this specific 3rd party app. This part is called **Authorization**, shortened to **authz**.

These permissions come in packages, for example you cannot define a 3rd party app that can only have access to your Firstname and not Lastname. These packages are called **scope** and you pick the most restrictive scope the 3rd party app needs. 


## The login flow

- User hits login on the app page
- The app page redirects user to a Google login page where Google asks for confirmation (of the requested scope)
- User is redirected to this app's return page along with a temporary Authorization Code
- The return page first uses this Authorization Code to fetch a temporary Access Token from Google
- This Access Token is then used to fetch Google objects that the app has permissions to


## Prerequisites

- Pick a scope to use. Try the minimum unless you know what you need: https://www.googleapis.com/auth/userinfo.profile
- Create Google API Credentials and Oauth Consent Screen: https://console.cloud.google.com/apis/dashboard
- Note the Client ID and Client Secret. Your app will use them on the login page


## What each page should do

- The login page offers a login link to a Google URL along with an application identifier
- The return page, before drawing any html, uses the Authorization Code to fetch an Access Token from Google.
-- If this Authorization Code is invalid, block the user with an error html
-- If an Access Token is returned, request Google objects if needed like name, profile pic etc.
-- Display the app html while maintaining a session ID


## Basic structure of the App return page

```
error_page="<p>Authentication error, please try logging in again</p>"
app_page="<p>You are now logged in</p>"

if (!code) {
  return error_page
  }
else {
  access_token = REST.request("https://accounts.google.com/o/oauth2/v2/auth",code=code)
  }
if (!access_token) {
  return error_page
  }
else {
  return app_page
}
```


<https://developers.google.com/identity/protocols/oauth2>
<https://developers.google.com/identity/protocols/oauth2/web-server>
<https://developers.google.com/identity/protocols/oauth2/scopes>



{% highlight ruby %}
def print_hi(name)
  puts "Hi, #{name}"
end
print_hi('Tom')
#=> prints 'Hi, Tom' to STDOUT.
{% endhighlight %}


