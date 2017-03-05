# IGR203
## Guillaume Lagrange

QT version : 5.5.1
A priori fonctionne sur les machines de l'école (testé en ssh).

Tout a été traité, j'ai juste un soucis que je ne comprends pas : il faut
parfois rappuyer sur stop malgré le fait que l'on soit dans l'état idle pour
continuer les transitions normales, cela intervient quand on a touché au dial
principalement, je ne sais pas d'où ça vient, tout semble correct dans mes
transitions.

La seule fonctionnalité "en plus" est le fait qu'en touchang le dial alors que
l'on est en mode idle, on part directement en mode timer select, et quand on
appuie sur start la cuisson commence avec le timer selectionné. J'ai fait cela
car c'est ainsi que mon micro-ondes fonctionne.
