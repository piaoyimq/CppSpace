class goods:
    def __init__(self, price):
        self.price = price

    def sale(self, amount):
        return self.price * amount

    def say(self):
        print "hello, I'm from python."

