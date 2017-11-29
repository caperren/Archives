import webapp2
from google.appengine.ext import ndb
import json

REQUIRED_POST_VALUES = {"slip_id", "arrival_date"}


class MarinaHandler(webapp2.RequestHandler):
    def post(self, boat_id):
        parsed_json = json.loads(self.request.body)

        if not REQUIRED_POST_VALUES <= set(parsed_json):
            self.error(404)
            self.response.write("POST missing one or more required fields...")
            return

        # First check to make sure the boat exists, and if so, get a reference to it.
        key = ndb.Key(urlsafe=boat_id)
        boat_to_dock = key.get()

        if not boat_to_dock:
            self.error(404)
            return

        # Then, we're also going to check to make sure the slip exists, and if so get a reference to it.
        key = ndb.Key(urlsafe=parsed_json["slip_id"])
        slip_to_view_raw = key.get()

        if not slip_to_view_raw:
            self.error(404)
            return

        # Now, we're going to check to make sure the slip isn't already occupied.
        if slip_to_view_raw.current_boat:
            self.error(403)
            return

        # Since all the data is valid, we're going to dock the boat.
        # Fist assign the boat to the slip, and update the arrival date
        slip_to_view_raw.current_boat = boat_to_dock.id
        slip_to_view_raw.arrival_date = parsed_json["arrival_date"]
        slip_to_view_raw.put()

        # Now change the boat to NOT be at sea anymore
        boat_to_dock.at_sea = False
        boat_to_dock.put()

        # Let's return the boat and slip information just for verification
        output_dict = {"slip": slip_to_view_raw.to_dict(), "boat": boat_to_dock.to_dict()}

        self.response.write(json.dumps(output_dict))

    def get(self, slip_id):
        key = ndb.Key(urlsafe=slip_id)
        slip_to_view_raw = key.get()

        if not slip_to_view_raw:
            self.error(404)
            return

        if not slip_to_view_raw.current_boat:
            self.error(404)
            return

        key = ndb.Key(urlsafe=slip_to_view_raw.current_boat)
        boat_to_view_raw = key.get()

        boat_to_view_dict = boat_to_view_raw.to_dict()
        self.response.write(json.dumps(boat_to_view_dict))

