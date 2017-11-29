#!/usr/bin/env python

# Breakdown of required URLS
# /boats/{id} -- Specific boat
# /boats -- All boats
# /slips/{id} -- Specific slip
# /slips -- All slips

# Operations Needed
# Add (POST)
# Delete (DELETE)
# Modify (PATCH)
# Replace (PUT)
# View (GET)

import webapp2

from BoatHandleCore import BoatHandler
from SlipHandlerCore import SlipHandler
from MarinaHandlerCore import MarinaHandler


class MainHandler(webapp2.RequestHandler):
    def get(self):
        self.response.write(
            "This is the main page for the REST Implementation and Planning assignment in CS496."
            "<br>Written by Corwin Perren.")

allowed_methods = webapp2.WSGIApplication.allowed_methods
new_allowed_methods = allowed_methods.union(('PATCH',))
webapp2.WSGIApplication.allowed_methods = new_allowed_methods
app = webapp2.WSGIApplication([
    webapp2.Route("/", MainHandler, "main-handler"),
    webapp2.Route("/slips", SlipHandler, "slip-handler"),
    webapp2.Route("/slips/<slip_id>", SlipHandler, "slip-handler"),
    webapp2.Route("/slips/<slip_id>/boat", MarinaHandler, "marina-handler"),
    webapp2.Route("/boats", BoatHandler, "boat-handler"),
    webapp2.Route("/boats/<boat_id>", BoatHandler, "boat-handler"),
    webapp2.Route("/boats/<boat_id>/slip", MarinaHandler, "marina-handler")
], debug=True)
