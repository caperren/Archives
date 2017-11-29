#!/usr/bin/env python

import webapp2
import jinja2
from webapp2_extras import sessions
import os
from google.appengine.api import urlfetch
import json

CLIENT_ID = "196055969432-4h2b1f38v094hs83ea7vafjul25vtf1a.apps.googleusercontent.com"
CLIENT_SECRET = "4rA3Oakv4jAJZCn1s6CN1kQn"

GOOGLE_OAUTH_URI = "https://accounts.google.com/o/oauth2/v2/auth"
GOOGLE_TOKEN_URI = "https://www.googleapis.com/oauth2/v4/token"
LOCAL_GACCOUNT_URI = "/gaccount"
GOOGLE_PLUS_URI = "https://www.googleapis.com/plus/v1/people/me"

RESPONSE_TYPE = "code"
REDIRECT_URI = "https://cs496-oauth2-174222.appspot.com/oauth"
SCOPES = "email"
GRANT_TYPE = "authorization_code"

jinja_environment = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.dirname(__file__) + "/framework"))


class BaseHandler(webapp2.RequestHandler):
    def dispatch(self):
        # Get a session store for this request.
        self.session_store = sessions.get_store(request=self.request)

        try:
            # Dispatch the request.
            webapp2.RequestHandler.dispatch(self)
        finally:
            # Save all sessions.
            self.session_store.save_sessions(self.response)

    @webapp2.cached_property
    def session(self):
        # Returns a session using the default cookie key.
        return self.session_store.get_session()


class IndexHandler(BaseHandler):
    def get(self):
        template = jinja_environment.get_template("index.html")
        self.response.write(template.render())


class LocalAuthHandler(BaseHandler):
    def get(self):
        state = self.request.get("state")

        if not state:
            self.error(404)
            self.response.write("Missing state parameter...")
            return

        self.session["state"] = state

        redirect_uri = GOOGLE_OAUTH_URI + "?response_type=" + RESPONSE_TYPE + "&client_id=" + CLIENT_ID + \
                       "&redirect_uri=" + REDIRECT_URI + "&scope=" + SCOPES + "&state=" + state

        self.redirect(str(redirect_uri))


class OauthHandler(BaseHandler):
    def get(self):
        state_session = self.session.get("state")
        state_url = self.request.get("state")

        if state_session != state_url:
            self.error(404)
            self.response.write("State response from OAUTH server does not match session state!")
            return

        code = self.request.get("code")

        url_to_post = GOOGLE_TOKEN_URI + "?code=" + code + "&client_id=" + CLIENT_ID + "&client_secret=" \
                      + CLIENT_SECRET + "&redirect_uri=" + REDIRECT_URI + "&grant_type=" + GRANT_TYPE

        post_response = urlfetch.fetch(url=url_to_post, method="POST")

        json_post_response = json.loads(post_response.content)

        if "access_token" in json_post_response:
            self.session["access_token"] = json_post_response["access_token"]
            self.redirect(LOCAL_GACCOUNT_URI)
        else:
            self.error(404)
            self.response.write("No authorization given.")


class GAccountHandler(BaseHandler):
    def get(self):
        state_session = self.session.get("state")
        access_token = self.session.get("access_token")

        if state_session and access_token:

            headers = {
                "Authorization": "Bearer " + access_token
            }

            google_plus_response = urlfetch.fetch(url=GOOGLE_PLUS_URI, headers=headers)
            gplus_dict = json.loads(google_plus_response.content)

            variables = {
                "first_name": "Google Plus not enabled on the user's profile...",
                "last_name": "Google Plus not enabled on the user's profile...",
                "gplus_url": "Google Plus not enabled on the user's profile...",
                "state_var": state_session

            }

            if "url" in gplus_dict:
                variables["first_name"] = gplus_dict["name"]["givenName"]
                variables["last_name"] = gplus_dict["name"]["familyName"]
                variables["gplus_url"] = gplus_dict["url"]

            template = jinja_environment.get_template("gaccount.html")
            self.response.write(template.render(**variables))
        else:
            template = jinja_environment.get_template("notlinked.html")
            self.response.write(template.render())

config = {'webapp2_extras.sessions': {
    'secret_key': 'NatEf7n0Ti-CoPAggdMph',
}}

app = webapp2.WSGIApplication([
    webapp2.Route("/oauth", OauthHandler, "main-handler"),
    webapp2.Route("/lauth", LocalAuthHandler, "local-auth-handler"),
    webapp2.Route("/gaccount", GAccountHandler, "gaccount-auth-handler"),
    webapp2.Route("/", IndexHandler, "index-handler")
], debug=True, config=config)
