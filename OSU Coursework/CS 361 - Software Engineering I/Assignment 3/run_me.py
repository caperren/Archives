# For testing implementation
from kwic import Kwic

output_check = [(['and', 'that', 'pair', 'So', 'is', 'this', 'pair'], 1),   (['good.', 'This', 'pair?', 'is'], 0),   (['is', 'good.', 'This', 'pair?'], 0),   (['is', 'this', 'pair', 'and', 'that', 'pair', 'So'], 1),   (['pair', 'and', 'that', 'pair', 'So', 'is', 'this'], 1),   (['pair', 'So', 'is', 'this', 'pair', 'and', 'that'], 1),   (['pair?', 'is', 'good.', 'This'], 0),   (['So', 'is', 'this', 'pair', 'and', 'that', 'pair'], 1),   (['that', 'pair', 'So', 'is', 'this', 'pair', 'and'], 1),   (['this', 'pair', 'and', 'that', 'pair', 'So', 'is'], 1),   (['This', 'pair?', 'is', 'good.'], 0)]
output_check_pairs = [(('is', 'pair'), 2), (('is', 'this'), 2), (('pair', 'this'), 2)]

if __name__ == "__main__":
    kc = Kwic()
    kc.addText("This pair? is good.\n So is this pair and that pair")
    print kc.listPairs() == output_check_pairs
    print kc.index() == output_check
    kc.print_eventspec_log()