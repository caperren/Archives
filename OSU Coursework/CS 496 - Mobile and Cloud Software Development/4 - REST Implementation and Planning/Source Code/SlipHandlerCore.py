import webapp2
from google.appengine.ext import ndb
import json

REQUIRED_POST_VALUES = {"number"}
DESIRED_BOAT_LIST_KEYS = ["number", "current_boat", "arrival_date", "id"]
AVAILABLE_KEYS_TO_REPLACE_MODIFY = ["number", "current_boat", "arrival_date"]


class Slip(ndb.Model):
    number = ndb.IntegerProperty(required=True)
    current_boat = ndb.StringProperty()
    arrival_date = ndb.StringProperty()
    id = ndb.StringProperty()


class SlipHandler(webapp2.RequestHandler):
    def get(self, slip_id=None):

        # If we have an id, show only the one boat
        if slip_id:
            key = ndb.Key(urlsafe=slip_id)

            slip_to_view_raw = key.get()

            if not slip_to_view_raw:
                self.error(404)
                return

            slip_to_view_dict = slip_to_view_raw.to_dict()

            self.response.write(json.dumps(slip_to_view_dict))

        # If we have no id, show all boats
        else:
            slips = Slip.query().fetch()

            output_array = []

            for slip in slips:
                slip_dict = slip.to_dict()
                temp_dict = {current_key: slip_dict[current_key] for current_key in DESIRED_BOAT_LIST_KEYS}
                output_array.append(temp_dict)

            self.response.write(json.dumps(output_array))

    def post(self):
        parsed_json = json.loads(self.request.body)

        if not REQUIRED_POST_VALUES <= set(parsed_json):
            self.error(404)
            self.response.write("POST missing one or more required fields...")
            return

        desired_slip_number = parsed_json["number"]

        slip_number_exists = len(Slip.query(Slip.number == desired_slip_number).fetch()) > 0

        if slip_number_exists:
            self.error(404)
            self.response.write("POST for slip number " + str(desired_slip_number) + " failed. Slip already exists.")
            return

        new_slip = Slip(number=desired_slip_number, current_boat=None, arrival_date=None)
        new_slip.put()

        new_slip.id = new_slip.key.urlsafe()
        new_slip.put()

        self.response.write(json.dumps(new_slip.to_dict()))

    def delete(self, slip_id):
        key = ndb.Key(urlsafe=slip_id)
        slip_to_delete = key.get()

        if not slip_to_delete:
            self.error(404)
            return

        # Put boat at sea if slip occupied
        if slip_to_delete.current_boat:
            key = ndb.Key(urlsafe=slip_to_delete.current_boat)
            boat_to_undock = key.get()
            if boat_to_undock:
                boat_to_undock.at_sea = True
                boat_to_undock.put()

        slip_to_delete.key.delete()

    def put(self, slip_id):
        key = ndb.Key(urlsafe=slip_id)

        slip_to_replace_raw = key.get()

        if not slip_to_replace_raw:
            self.error(404)
            return

        parsed_json = json.loads(self.request.body)

        if not len(parsed_json):
            self.error(404)
            self.response.write("PUT missing parameters to replace...")
            return

        temp_dict = {current_key: parsed_json[current_key] for current_key in AVAILABLE_KEYS_TO_REPLACE_MODIFY}
        slip_to_replace_raw.populate(**temp_dict)

        slip_to_replace_raw.put()

        self.response.write(json.dumps(slip_to_replace_raw.to_dict()))

    def patch(self, slip_id):
        key = ndb.Key(urlsafe=slip_id)

        slip_to_modify_raw = key.get()

        if not slip_to_modify_raw:
            self.error(404)
            return

        parsed_json = json.loads(self.request.body)

        if not len(parsed_json) or len(parsed_json) > 1:
            self.error(404)
            self.response.write("PATCH has incorrect number of parameters to modify...")
            return

        key, _ = parsed_json.items()[0]

        if key in AVAILABLE_KEYS_TO_REPLACE_MODIFY:
            slip_to_modify_raw.populate(**parsed_json)

            slip_to_modify_raw.put()

            self.response.write(json.dumps(slip_to_modify_raw.to_dict()))

        else:
            self.error(404)
            self.response.write("PATCH parameter not valid...")
