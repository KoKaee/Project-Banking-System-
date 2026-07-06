# Project-Banking-System

Système Bancaire Hybride (C / Python) — Projet NF06A

Ce dépôt contient le projet de programmation réalisé dans le cadre de l'UV NF06A (Pratique de la programmation) à l'Université de Technologie de Troyes (UTT).

L'objectif de ce projet est de développer un système bancaire complet, associant la robustesse et la performance d'un back-end en C (pour la gestion dynamique des structures de données et de la logique métier) à la flexibilité d'un front-end en Python sous forme d'interface en ligne de commande (CLI).

Fonctionnalités Principales
Le système gère deux types d'utilisateurs (Clients et Conseillers Bancaires) et prend en charge les opérations suivantes :
Pour les Clients
  Authentification sécurisée (Login / Logout).
  Consultation et mise à jour des détails personnels.
  Opérations financières : Dépôts, retraits (avec vérification du solde) et virements interbancaires via le numéro RIB.
  Simulation de crédit : Calcul automatique de la capacité d'emprunt maximale (limitée à $33\%$ du salaire mensuel net, charges déduites).
  Communication : Envoi de messages directs au conseiller attitré.

Pour les Conseillers Bancaires
  Accès complet aux fonctionnalités et données des clients qui leur sont assignés.
  Gestion de la clientèle : Ajout et suppression de comptes clients.
  Messagerie : Lecture centralisée de tous les messages reçus de la part de leurs clients.
