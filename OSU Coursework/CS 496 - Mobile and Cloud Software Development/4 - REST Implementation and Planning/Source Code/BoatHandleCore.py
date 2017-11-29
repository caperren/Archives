import webapp2
from google.appengine.ext import ndb
import json

from SlipHandlerCore import Slip

REQUIRED_POST_VALUES = {"name", "type", "length"}
DESIRED_BOAT_LIST_KEYS = ["name", "type", "length", "at_sea", "id"]
AVAILABLE_KEYS_TO_REPLACE_MODIFY = ["name", "type", "length", "at_sea"]


class Boat(ndb.Model):
    name = ndb.StringProperty(required=True)
    type = ndb.StringProperty(required=True)
    length = ndb.IntegerProperty(required=True)
    at_sea = ndb.BooleanProperty(required=True)
    id = ndb.StringProperty()


class BoatHandler(webapp2.RequestHandler):
    def get(self, boat_id=None):

        # If we have an id, show only the one boat
        if boat_id:
            key = ndb.Key(urlsafe=boat_id)

            boat_to_view_raw = key.get()

            if not boat_to_view_raw:
                self.error(404)
                return

            boat_to_view_dict = boat_to_view_raw.to_dict()

            self.response.write(json.dumps(boat_to_view_dict))

        # If we have no id, show all boats
        else:
            boats = Boat.query().fetch()

            output_array = []

            for boat in boats:
                boat_dict = boat.to_dict()
                temp_dict = {current_key: boat_dict[current_key] for current_key in DESIRED_BOAT_LIST_KEYS}
                output_array.append(temp_dict)

            self.response.write(json.dumps(output_array))

    def post(self):
        parsed_json = json.loads(self.request.body)

        if not REQUIRED_POST_VALUES <= set(parsed_json):
            self.error(404)
            self.response.write("POST missing one or more required fields...")
            return

        boat_name = parsed_json["name"]
        boat_type = parsed_json["type"]
        boat_length = parsed_json["length"]

        new_boat = Boat(name=boat_name, type=boat_type, length=boat_length, at_sea=True)
        new_boat.put()

        new_boat.id = new_boat.key.urlsafe()
        new_boat.put()

        self.response.write(json.dumps(new_boat.to_dict()))

    def delete(self, boat_id):
        # Check to make sure the boat is valid.
        key = ndb.Key(urlsafe=boat_id)
        boat_to_delete = key.get()

        if not boat_to_delete:
            self.error(404)
            return

        # Check if the boat is currently docked in a slip, if so, undock it.
        slips = Slip.query(Slip.current_boat == boat_id).fetch()

        for slip in slips:
            slip.current_boat = None
            slip.arrival_date = None
            slip.put()

        boat_to_delete.key.delete()

    def put(self, boat_id):
        key = ndb.Key(urlsafe=boat_id)

        boat_to_replace_raw = key.get()

        if not boat_to_replace_raw:
            self.error(404)
            return

        parsed_json = json.loads(self.request.body)

        if not len(parsed_json):
            self.error(404)
            self.response.write("PUT missing parameters to replace...")
            return

        if "at_sea" in parsed_json:
            if parsed_json["at_sea"] is True and boat_to_replace_raw.at_sea is False:
                # Undock the ship, as we're leaving
                slips = Slip.query(Slip.current_boat == boat_id).fetch()

                for slip in slips:
                    slip.current_boat = None
                    slip.arrival_date = None
                    slip.put()

        temp_dict = {current_key: parsed_json[current_key] for current_key in AVAILABLE_KEYS_TO_REPLACE_MODIFY}
        boat_to_replace_raw.populate(**temp_dict)
        boat_to_replace_raw.put()

        self.response.write(json.dumps(boat_to_replace_raw.to_dict()))

    def patch(self, boat_id):
        key = ndb.Key(urlsafe=boat_id)

        boat_to_modify_raw = key.get()

        if not boat_to_modify_raw:
            self.error(404)
            return

        parsed_json = json.loads(self.request.body)

        if not len(parsed_json) or len(parsed_json) > 1:
            self.error(404)
            self.response.write("PATCH has incorrect number of parameters to modify...")
            return

        key, _ = parsed_json.items()[0]

        if key in AVAILABLE_KEYS_TO_REPLACE_MODIFY:
            if "at_sea" in parsed_json:
                if parsed_json["at_sea"] is True and boat_to_modify_raw.at_sea is False:
                    # Undock the ship, as we're leaving
                    slips = Slip.query(Slip.current_boat == boat_id).fetch()

                    for slip in slips:
                        slip.current_boat = None
                        slip.arrival_date = None
                        slip.put()

            boat_to_modify_raw.populate(**parsed_json)
            boat_to_modify_raw.put()

            self.response.write(json.dumps(boat_to_modify_raw.to_dict()))

        else:
            self.error(404)
            self.response.write("PATCH parameter not valid...")
