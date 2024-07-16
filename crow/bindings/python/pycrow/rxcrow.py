from pycrow.pubsub import publish, subscribe
import reactivex
import reactivex.subject


def rxsubscribe(theme, keepalive=2000):
    subject = reactivex.subject.Subject()
    subscribe(theme, lambda x: subject.on_next(x), keepalive)
    return subject


def rxpublish(theme, observable):
    observable.subscribe(lambda x: publish(theme, x))
