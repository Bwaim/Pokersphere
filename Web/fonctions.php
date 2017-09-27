<?php
	
	function select($numReq, $franchise)
	{
		switch ($numReq)
		{
			case 1:
				$req[0] = "select u.ID, u.LOGIN, u.PASSWORD, g.ID AS ID_GROUP, g.NAME
				from TB01_USERS u, TB02_GROUP g
				WHERE u.GROUP_ID = g.ID
				AND u.LOGIN = '?0?'
				AND u.PASSWORD = SHA1('?1?')
				AND g.NAME = 'Administrateurs'";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND POKERSPHERE_ID = $franchise";
				}
				
				break;
				
			case 2:
				$req[0] = "select m.ID, m.NAME Nom, m.FIRSTNAME Prenom, u.LOGIN `Id Utilisateur`, T9.PSEUDO `Id PMU`
				, DATE_FORMAT(m.BIRTH_DATE,'%d/%m/%Y') `Date de Naissance`, m.ADRESS Adresse
				, m.ZIP_CODE `Code Postal`, m.CITY Ville, m.PHONE Portable, m.EMAIL Mail, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance
				, DATE_FORMAT(m.CREATION_DATE,'%d/%m/%Y') Anciennete, m.Kosmos, m.CLUB, a.VALUE RC, CARD_NUMBER `N° de Carte`
				, s.NAME `Nom Parrain`, s.FIRSTNAME `Prenom Parrain`, s.ID SPONSOR_ID, pp.LIBELLE Franchise, m.PLAQUE_IMMATRICULATION `Plaque d'immatriculation`
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select distinct a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN TB13_MEMBER_CARD TB13
					ON m.ID = TB13.ID_MEMBER
					LEFT OUTER JOIN TB01_USERS u
					ON u.ID_MEMBER = m.ID
					LEFT OUTER JOIN TB03_MEMBER s
					ON m.ID_SPONSOR = s.ID
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
					LEFT OUTER JOIN TB09_MEMBER_ROOM T9
					ON T9.ID_MEMBER = m.ID
					AND T9.ID_ROOM = 8
				WHERE m.DELETED = 'N'";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				
				$req[0] .= " ORDER BY m.NAME, m.FIRSTNAME";
				break;
				
			case 3:
				$req[0] = "select a.ID, a.VALUE `Type d'abonnement`, DATE_FORMAT(a.OPERATION_DATE,'%d/%m/%Y %H:%i') `Date de l'operation`
				, DATE_FORMAT(a.ENABLE_DATE,'%d/%m/%Y') `Debut de l'abonnement`, DATE_FORMAT(a.DISABLE_DATE,'%d/%m/%Y') `Fin de l'abonnement`
				, PRIX Prix
				from TB04_ADHESION a
				WHERE a.ID_MEMBER = ?0?
				ORDER BY a.OPERATION_DATE";
				break;
			
			case 4:
				$req[0] = "select m.ID, pp.LIBELLE Franchise, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo, m.Kosmos
				, CASE WHEN m.KOSMOS < ROUND((m.KOSMOS - IFNULL(k1.netResult,0)) / 100 * 5) + IFNULL(k1.netResult,0) 
					   THEN m.KOSMOS
					   ELSE ROUND((m.KOSMOS - IFNULL(k1.netResult,0)) / 100 * 5) + IFNULL(k1.netResult,0)
					   END `Kosmos Disponible`
				, m.KOSMOS - IFNULL(k2.KOSMOS_ADHESION,0) `Kosmos aux Enchères`
				, m.KOSMOS + IFNULL(k3.KOSMOS_RESULTAT_NET,0) `Resultat Net`
				, IFNULL(k4.KOSMOS_RESULTAT_MOIS_PREC,0) `Resultat Mois precedent`
				, IFNULL(k5.KOSMOS_RESULTAT_MOIS_COURANT,0) `Resultat Mois en cours`
				, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance, a.VALUE RC
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE,
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN
					(select k.ID_MEMBER, SUM(CASE WHEN k.CREDIT = 1 THEN k.VALUE ELSE -k.VALUE END) netResult
					 FROM TB05_KOSMOS k
					 WHERE k.REASON NOT in (3,4,5)
					 AND k.OPERATION_DATE >= CASE WHEN HOUR(NOW()) < 8 
												  THEN DATE_ADD(DATE_SUB(CURDATE(),INTERVAL 1 DAY), INTERVAL 8 HOUR)
												  ELSE DATE_ADD(CURDATE(),INTERVAL 8 HOUR)
											 END
					 GROUP BY k.ID_MEMBER) k1
					ON m.ID = k1.ID_MEMBER
					LEFT OUTER JOIN
					(select k.ID_MEMBER, SUM(k.VALUE) KOSMOS_ADHESION
					 FROM TB05_KOSMOS k
					 WHERE k.REASON = 4
					 GROUP BY k.ID_MEMBER) k2
					ON m.ID = k2.ID_MEMBER
					LEFT OUTER JOIN
					(select k.ID_MEMBER, SUM(CASE WHEN k.REASON = 3 THEN k.VALUE ELSE -k.VALUE END) KOSMOS_RESULTAT_NET
					 FROM TB05_KOSMOS k
					 WHERE k.REASON IN (3,4,5)
					 GROUP BY k.ID_MEMBER) k3
					ON m.ID = k3.ID_MEMBER
					LEFT OUTER JOIN
					(select k.ID_MEMBER, SUM(CASE WHEN k.CREDIT = 1 THEN k.VALUE ELSE -k.VALUE END) KOSMOS_RESULTAT_MOIS_PREC
					 FROM TB05_KOSMOS k
					 WHERE k.REASON NOT in (3,4,5)
					 AND k.OPERATION_DATE >= DATE_ADD(DATE_SUB(DATE_ADD(CURDATE(), INTERVAL 8 HOUR),INTERVAL 1 MONTH),INTERVAL 1-DAYOFMONTH(CURDATE()) DAY)
					 AND k.OPERATION_DATE < DATE_ADD(DATE_ADD(CURDATE(), INTERVAL 8 HOUR),INTERVAL 1-DAYOFMONTH(CURDATE()) DAY)
					 GROUP BY k.ID_MEMBER) k4
					ON m.ID = k4.ID_MEMBER
					LEFT OUTER JOIN
					(select k.ID_MEMBER, SUM(CASE WHEN k.CREDIT = 1 THEN k.VALUE ELSE -k.VALUE END) KOSMOS_RESULTAT_MOIS_COURANT
					 FROM TB05_KOSMOS k
					 WHERE k.REASON NOT in (3,4,5)
					 AND k.OPERATION_DATE >= DATE_ADD(DATE_ADD(CURDATE(), INTERVAL 8 HOUR),INTERVAL 1-DAYOFMONTH(CURDATE()) DAY)
					 AND k.OPERATION_DATE < DATE_ADD(DATE_ADD(DATE_ADD(CURDATE(), INTERVAL 8 HOUR),INTERVAL 1-DAYOFMONTH(CURDATE()) DAY), INTERVAL 1 MONTH)
					 GROUP BY k.ID_MEMBER) k5
					ON m.ID = k5.ID_MEMBER
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N'";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				
				$req[0] .= " ORDER BY m.NAME, m.FIRSTNAME";
				break;
			
			case 5:
				$req[0] = "select k.ID, k.VALUE `Valeur`, DATE_FORMAT(k.OPERATION_DATE,'%d/%m/%Y %H:%i') `Date de l'operation`
				, k.REASON `Type d'operation`, k.COMMENT `Infos Additionnelles`, k.CREDIT
				from TB05_KOSMOS k
				WHERE k.ID_MEMBER = ?0?
				ORDER BY k.OPERATION_DATE";
				break;
				
			case 6:
				$req[0] = "select t.ID, t.NAME `Tournoi`, DATE_FORMAT(t.BEGIN_DATE,'%d/%m/%Y %H:%i') `Debut du tournoi`
				, DATE_FORMAT(t.END_REGISTRATION_DATE,'%d/%m/%Y %H:%i') `Fin des inscriptions`, t.BUY_IN `Buy-in`
				, t.NB_PLAYERS `Nombre de joueurs`, t.Variante
				, (select count('x') 
				   from TB07_TOURNAMENT_REGISTRATION tr, TB03_MEMBER m
				   where ID_TOURNAMENT = t.ID and tr.ID_MEMBER = m.id and m.DELETED = 'N') `Nombre d'inscrits`
				, CASE t.PRIVATE WHEN 0 THEN 'Non' ELSE 'Oui' END Prive
				, t.COMMENT `Commentaire`
				, t.SENT_TO_RANKINGHERO
				, t.VALIDATED, t.POKERSPHERE_ID Franchise, t.ATTACHED_TOURNAMENT `Tournoi lie` 
				from TB06_TOURNAMENT t
				WHERE t.BEGIN_DATE BETWEEN str_to_date('?0?','%d/%m/%Y') AND str_to_date('?1?','%d/%m/%Y')
				AND t.TYPE = ?2?";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND t.POKERSPHERE_ID = $franchise";
				}
				
				$req[0] .= " ORDER BY t.BEGIN_DATE";
				break;
				
			case 7:
				$req[0] = "select m.ID, pp.LIBELLE Franchise, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo, m.Kosmos
				, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance, tr.POSITION Place, tr.IS_ARRIVED, tr.IS_LATE
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
					, TB07_TOURNAMENT_REGISTRATION tr
				WHERE m.DELETED = 'N' and tr.ID_MEMBER = m.id and tr.ID_TOURNAMENT = ?0?
				ORDER BY tr.POSITION, m.NAME, m.FIRSTNAME";
				break;
			
			case 8:
				$req[0] = "select m.ID, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo, m.Kosmos
				, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance, pp.LIBELLE Franchise
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N' AND m.NAME like '?0?%'";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				
				$req[0] .= " ORDER BY m.NAME, m.FIRSTNAME";
				break;
			
			case 9:
				$req[0] = "select CASE WHEN count('x') > 0 THEN 'KO' ELSE 'OK' END RESULT
				from TB05_KOSMOS k";
				if ($franchise > 0)
				{
					$req[0] .= " , TB03_MEMBER m";
				}
				$req[0] .= " WHERE COMMENT like CONCAT('%', MONTHNAME(str_to_date('?0?','%d/%m/%Y')), '%'
							 , YEAR(str_to_date('?0?','%d/%m/%Y')))
				AND k.REASON = 4 AND k.VALUE = 1000";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND k.ID_MEMBER = m.ID
					AND m.POKERSPHERE_ID = $franchise";
				}
				
				break;
			
			case 10:
				$req[0] = "select (select count('x') from TB03_MEMBER WHERE DELETED = 'N'";
				if ($franchise > 0)
				{
					$req[0] .= " AND POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ) NB_ADHERENT,
				
				(select count(distinct a.ID_MEMBER) from TB04_ADHESION a";
				if ($franchise > 0)
				{
					$req[0] .= " , TB03_MEMBER m";
				}
				$req[0] .= " where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)
					 AND CURDATE() < a.DISABLE_DATE";
				if ($franchise > 0)
				{
					$req[0] .= " AND a.ID_MEMBER = m.ID
					AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ) NB_ADHERENT_A_JOUR,
				
				(select count(distinct a.ID_MEMBER) from TB04_ADHESION a";
				if ($franchise > 0)
				{
					$req[0] .= " , TB03_MEMBER m";
				}
				$req[0] .= " where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)
					 AND a.DISABLE_DATE BETWEEN CURDATE() AND DATE_ADD(CURDATE(), INTERVAL 7 DAY) ";
				if ($franchise > 0)
				{
					$req[0] .= " AND a.ID_MEMBER = m.ID
					AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= ") NB_ADHERENT_QUI_EXPIRE,
					 
				(select count(distinct a.ID_MEMBER) from TB04_ADHESION a";
				if ($franchise > 0)
				{
					$req[0] .= " , TB03_MEMBER m";
				}
				$req[0] .= " where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)
					 AND CURDATE() > a.DISABLE_DATE";
				if ($franchise > 0)
				{
					$req[0] .= " AND a.ID_MEMBER = m.ID
					AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= ") NB_ADHERENT_EXPIRES";
				break;
			
			case 11:
				$req[0] = "select SUM(t.BUY_IN) `Prizepool`, count('x') `Nombre d'inscrits`
				from TB06_TOURNAMENT t,
				TB07_TOURNAMENT_REGISTRATION tr
				WHERE t.ID = tr.ID_TOURNAMENT
				AND t.ID = ?0?";
				break;
			
			case 12 :
				$req[0] = "select m.NAME, m.FIRSTNAME, m.EMAIL, m.PHONE
				from TB03_MEMBER m
				WHERE m.DELETED = 'N'";
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ORDER BY NAME, FIRSTNAME";
				break;
			
			case 13 :
				$req[0] = "select m.NAME, m.FIRSTNAME, m.EMAIL, m.PHONE
				from TB03_MEMBER m,
				(select a.ID_MEMBER from TB04_ADHESION a
							   where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)
							   AND CURDATE() < a.DISABLE_DATE) a
				WHERE m.ID = a.ID_MEMBER
				AND m.DELETED = 'N'";
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ORDER BY NAME, FIRSTNAME";
				break;
			
			case 14 :
				$req[0] = "select m.NAME, m.FIRSTNAME, m.EMAIL, m.PHONE
				from TB03_MEMBER m,
				(select a.ID_MEMBER from TB04_ADHESION a
							   where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)
							   AND a.DISABLE_DATE BETWEEN CURDATE() AND DATE_ADD(CURDATE(), INTERVAL 7 DAY) ) a
				WHERE m.ID = a.ID_MEMBER
				AND m.DELETED = 'N'";
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ORDER BY NAME, FIRSTNAME";
				break;
				
			case 15 :
				$req[0] = "select m.NAME, m.FIRSTNAME, m.EMAIL, m.PHONE
				from TB03_MEMBER m,
				(select a.ID_MEMBER from TB04_ADHESION a
							   where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)
							   AND CURDATE() > a.DISABLE_DATE) a
				WHERE m.ID = a.ID_MEMBER
				AND m.DELETED = 'N'";
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ORDER BY NAME, FIRSTNAME";
				break;
			/*case 15 :
				$req[0] = "select m.ID, c.PICTURE
				from TB03_MEMBER m, contact c
				WHERE c.Nom = m.NAME
				AND c.Prenom = m.FIRSTNAME
				AND c.`E-Mail` = m.EMAIL";
				break;*/
				
			case 16 :
				//, IFNULL(TB5G.VALUE,0) - TB5B.VALUE 'Resultat net'
				$req[0] = "select T7.POSITION Position, m.NAME Nom, m.FIRSTNAME Prenom
				from TB03_MEMBER m,
				TB05_KOSMOS TB5B,
				TB07_TOURNAMENT_REGISTRATION T7
				LEFT OUTER JOIN
				TB05_KOSMOS TB5G
				ON T7.ID_GAIN = TB5G.ID
				WHERE T7.ID_TOURNAMENT = ?0?
				AND T7.ID_MEMBER = m.ID
				AND T7.ID_BUY_IN = TB5B.ID
				ORDER BY T7.POSITION, NAME, FIRSTNAME";
				break;
			
			case 17 :
				$req[0] = "SELECT DATE_FORMAT(OPERATION_DATE,'%Y%m') MOIS, 
					SUM(CASE WHEN VALUE = 0 THEN 1 ELSE 0 END) PASS_DECOUVERTE,
					SUM(CASE WHEN VALUE = 1 THEN 1 ELSE 0 END) ADHESION_1_MOIS,
					SUM(CASE WHEN VALUE = 2 THEN 1 ELSE 0 END) ADHESION_3_MOIS,
					SUM(CASE WHEN VALUE = 3 THEN 1 ELSE 0 END) ADHESION_6_MOIS,
					SUM(CASE WHEN VALUE = 4 THEN 1 ELSE 0 END) ADHESION_12_MOIS,
					SUM(CASE WHEN VALUE = 5 THEN 1 ELSE 0 END) CONVERSION,
					SUM(CASE WHEN VALUE = 6 THEN 1 ELSE 0 END) CONVERSION_REFUSEE,
					SUM(CASE WHEN VALUE = 7 THEN 1 ELSE 0 END) ADHESION_ONLINE,
					IFNULL(FIRST,0) PREMIER_ABONNEMENT
				FROM TB04_ADHESION 
				LEFT OUTER JOIN 
				(SELECT COUNT('x') FIRST, MONTH FROM (
				SELECT MIN(DATE_FORMAT(OPERATION_DATE,'%Y%m')) MONTH FROM `TB04_ADHESION`
				, TB03_MEMBER T3
				WHERE T3.ID = ID_MEMBER AND DELETED = 'N'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') >= '201001'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') <= DATE_FORMAT(CURDATE(),'%Y%m')";
				if ($franchise > 0)
				{
					$req[0] .= " AND T3.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " GROUP BY ID_MEMBER) FIRST_ADHESION
				GROUP BY MONTH) FIRST_ADHESION
				ON DATE_FORMAT(OPERATION_DATE,'%Y%m') = MONTH
				, TB03_MEMBER T3
				WHERE T3.ID = ID_MEMBER AND DELETED = 'N'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') >= '?0?'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') <= '?1?'";
				if ($franchise > 0)
				{
					$req[0] .= " AND T3.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " GROUP BY DATE_FORMAT(OPERATION_DATE,'%Y%m')	
				ORDER BY DATE_FORMAT(OPERATION_DATE,'%Y%m')";
				break;
				
			case 18:
				$req[0] = "select t.ID, t.NAME `Tournoi`, t.BUY_IN `Buy-in`
				, t.NB_PLAYERS `Nombre de joueurs`, t.Variante, t.COMMENT `Commentaire`
				from TB12_TOURNAMENT_TEMPLATE t";
				if ($franchise > 0)
				{
					$req[0] .= " WHERE t.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ORDER BY t.NAME";
				break;
				
			case 19:
				$req[0] = "select DATE_FORMAT(min(k.OPERATION_DATE),'%d/%m/%Y') DT_MIN
				, DATE_FORMAT(max(k.OPERATION_DATE),'%d/%m/%Y') DT_MAX
				from TB05_KOSMOS k
				WHERE k.ID_MEMBER = ?0?";
				break;
				
			case 20:
				$req[0] = "select k.ID Id, k.VALUE `Valeur`, DATE_FORMAT(k.OPERATION_DATE,'%d/%m/%Y %H:%i') `Date de l'operation`
				, CASE k.REASON 
				WHEN 0 THEN 'Tournoi MTT'
				WHEN 1 THEN 'Sit & Go'
				WHEN 2 THEN 'Partie Libre'
				WHEN 3 THEN 'Enchères'
				WHEN 4 THEN 'Adhesion'
				WHEN 5 THEN 'Bonus'
				END `Type d'operation`
				, k.COMMENT `Infos Additionnelles`, k.CREDIT Credit
				from TB05_KOSMOS k
				WHERE k.ID_MEMBER = ?0?
				AND k.OPERATION_DATE between str_to_date('?1?','%d/%m/%Y') AND str_to_date('?2?','%d/%m/%Y')
				AND k.REASON in (?3?)
				ORDER BY k.OPERATION_DATE";
				break;
				
			case 21:
				$req[0] = "SELECT COUNT(distinct ID_MEMBER) FROM (
				SELECT distinct ID_MEMBER
				FROM TB05_KOSMOS, TB03_MEMBER T3
				WHERE T3.ID = ID_MEMBER AND DELETED = 'N'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') >= '?0?'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') <= '?1?'";
				if ($franchise > 0)
				{
					$req[0] .= " AND T3.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " 
				UNION
				SELECT distinct ID_MEMBER FROM (
				SELECT ID_MEMBER, MIN(DATE_FORMAT(OPERATION_DATE,'%Y%m')) MONTH 
				FROM TB04_ADHESION, TB03_MEMBER T3
				WHERE T3.ID = ID_MEMBER AND DELETED = 'N'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') >= '201001'
				AND DATE_FORMAT(OPERATION_DATE,'%Y%m') <= DATE_FORMAT(CURDATE(),'%Y%m')";
				if ($franchise > 0)
				{
					$req[0] .= " AND T3.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= "
				GROUP BY ID_MEMBER) FIRST_ADHESION
				WHERE MONTH >= '?0?'
				AND MONTH <= '?1?') T1";
				break;
				
			case 22:
				$req[0] = "select m.ID, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo, m.Kosmos
				, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance, pp.LIBELLE Franchise
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N' AND m.NAME like '?0?%'
				AND m.ID not in (select ID_MEMBER FROM TB13_MEMBER_CARD WHERE CARD_NUMBER is NOT NULL)";
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= "
				ORDER BY m.NAME, m.FIRSTNAME";
				break;
			
			case 23:
				$req[0] = "select m.NAME Nom, m.FIRSTNAME Prenom, m.EMAIL Email, m.PHONE Portable
				from TB03_MEMBER m, TB07_TOURNAMENT_REGISTRATION TB07
				WHERE m.ID = ID_MEMBER
				AND ID_TOURNAMENT = ?0?
				ORDER BY m.NAME, m.FIRSTNAME";
				break;
				
			case 24:
				$req[0] = "select m.ID, m.NAME Nom, m.FIRSTNAME Prenom, pp.LIBELLE Franchise
				from TB03_MEMBER m
				LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N' AND m.NAME like '?0?%'";
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= "
				ORDER BY m.NAME, m.FIRSTNAME";
				break;
				
			case 25:
				$req[0] = "select ID, LAUNCHING Declenchement, TYPE_GODSON_GAIN `Type de gain filleul`, GODSON_GAIN `Gain filleul`
				, TYPE_SPONSOR_GAIN `Type de gain parrain`, SPONSOR_GAIN `Gain parrain`
				, NB_MONTH_FIRST_MEMBERSHIP `Nb mois 1er abonnement`
				from TB15_GAIN_SPONSOR";
				break;
			
			case 26:
				$req[0] = "select ID, LIBELLE Franchise
				from PSP_POKERSPHERE";
				break;
			
			case 27:
				$req[0] = "select CONCAT(T7.POSITION,' ',FIRSTNAME,' ',T3.NAME ,' ',IFNULL(VALUE,''), CASE WHEN VALUE IS NOT NULL AND T7_LIE.ID_MEMBER IS NOT NULL THEN ' Kosmos -' ELSE '' END, 
				CASE WHEN T7_LIE.ID_MEMBER IS NOT NULL THEN ' ticket tournoi' ELSE '' END) Label
				, T7.POSITION, FIRSTNAME, T3.NAME, VALUE, T3.ID
				from TB03_MEMBER T3,
				TB07_TOURNAMENT_REGISTRATION T7
				LEFT OUTER JOIN
				TB05_KOSMOS T5
				ON
				T7.ID_GAIN = T5.ID
				LEFT OUTER JOIN
				TB06_TOURNAMENT T6
				ON
				T6.ID = T7.ID_TOURNAMENT
				LEFT OUTER JOIN TB07_TOURNAMENT_REGISTRATION T7_LIE
				ON T6.ATTACHED_TOURNAMENT = T7_LIE.ID_TOURNAMENT
				AND T7.ID_MEMBER = T7_LIE.ID_MEMBER
				WHERE T7.ID_MEMBER = T3.ID
				AND T7.ID_TOURNAMENT = ?0?
				AND (T7.ID_GAIN IS NOT NULL OR T7_LIE.ID_MEMBER IS NOT NULL)
				ORDER BY T7.POSITION";
				break;
				
			case 28:
				$req[0] = "select m.ID, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo, m.Kosmos, pp.LIBELLE Franchise
				from TB03_MEMBER m
				LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N' AND m.ID in (?0?)";
				break;
				
			case 29:
				$req[0] = "select t.ID, CONCAT(t.NAME,' : ',DATE_FORMAT(t.BEGIN_DATE,'%d/%m/%Y %H:%i')) Nom
				from TB06_TOURNAMENT t
				WHERE BEGIN_DATE >= CURDATE()
				AND POKERSPHERE_ID = ?0?
				ORDER BY t.BEGIN_DATE";
				break;
				
			case 30:
				$req[0] = "select t.ID, CONCAT(t.NAME,' : ',DATE_FORMAT(t.BEGIN_DATE,'%d/%m/%Y %H:%i')) Nom
				from TB06_TOURNAMENT t
				WHERE BEGIN_DATE >= str_to_date('?1?','%d/%m/%Y')
				AND POKERSPHERE_ID = ?0?
				AND t.ID != ?2?
				AND t.TYPE = ?3?
				ORDER BY t.BEGIN_DATE";
				break;
				
			case 31:
				$req[0] = "select m.ID, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo, m.Kosmos
				, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance, pp.LIBELLE Franchise
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE, 
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N' AND m.NAME like '?0?%'";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID != $franchise";
				}
				
				$req[0] .= " ORDER BY m.NAME, m.FIRSTNAME";
				break;
				
			case 32:
				$req[0] = "select distinct c.CLUB
				from TB16_CLUB c";
				if ($franchise > 0)
				{
					$req[0] .= " WHERE c.POKERSPHERE_ID = $franchise";
				}
				$req[0] .= " ORDER BY c.CLUB";
				break;
				
			case 33:
				$req[0] = "select t.ID, t.STATUT `Statut`, t.SEUIL `Seuil`
				from TB17_FID_STATUT t
				ORDER BY t.SEUIL DESC";
				break;
				
			case 34:
				$req[0] = "select t.ID, t.NAME, t.RATIO, t.VALUE, t.NB_MONTH
				from TB18_FID_TYPE t";
				break;
				
			case 35:
				$req[0] = "select f.ID, f.NB `Valeur`, DATE_FORMAT(f.EVENT_DATE,'%d/%m/%Y %H:%i') `Date de l'operation`
				, f.ID_FID_TYPE `Type d'operation`, f.COMMENT `Infos Additionnelles`
				from TB19_FID_POINTS f
				WHERE f.ID_MEMBER = ?0?
				ORDER BY f.EVENT_DATE";
				break;
				
			case 36:
				$req[0] = "select m.ID, pp.LIBELLE Franchise, m.NAME Nom, m.FIRSTNAME Prenom, m.Pseudo
				, DATE_FORMAT(a.ECHEANCE,'%d/%m/%Y') Echeance, a.VALUE RC
				from TB03_MEMBER m
					LEFT OUTER JOIN
					(select a.ID_MEMBER, a.DISABLE_DATE ECHEANCE,
					 CASE WHEN CURDATE() BETWEEN a.ENABLE_DATE AND a.DISABLE_DATE THEN a.VALUE ELSE 0 END VALUE
					 from TB04_ADHESION a
					 where a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER)) a
					ON m.ID = a.ID_MEMBER
					LEFT OUTER JOIN PSP_POKERSPHERE pp
					ON m.POKERSPHERE_ID = pp.ID
				WHERE m.DELETED = 'N'";
				
				if ($franchise > 0)
				{
					$req[0] .= " AND m.POKERSPHERE_ID = $franchise";
				}
				
				$req[0] .= " ORDER BY m.NAME, m.FIRSTNAME";
				break;
				
			case 37:
				$req[0] = "select IFNULL(s.STATUT,'Pas de statut') Statut , ROUND(IFNULL(p.points, 0),0) Points,
				IFNULL(p2.pointsFid, 0) 'Points Fidelite'
				from
					(SELECT SUM(RATIO* CASE WHEN ID_FID_TYPE = 0 
					THEN NB / IFNULL((SELECT RATIO FROM TB18_FID_ABONNEMENT_RATIO 
						WHERE NB_MONTH = (SELECT CASE T4.VALUE WHEN 1 THEN 1 WHEN 2 THEN 3 WHEN 3 THEN 6 WHEN 4 THEN 12 END FROM TB04_ADHESION T4 WHERE T4.ID = ID_REF_OPERATION)),1)
					ELSE NB END) points
					FROM TB19_FID_POINTS T1, TB18_FID_TYPE T2
					WHERE ID_MEMBER = ?0?
					AND ID_FID_TYPE = T2.ID
					AND SYSDATE() <= CASE WHEN NB_MONTH = 0 THEN SYSDATE() ELSE DATE_ADD(EVENT_DATE, INTERVAL NB_MONTH MONTH) END
					) p
					LEFT OUTER JOIN TB17_FID_STATUT s
					ON SEUIL <= points,
					(SELECT SUM(CASE WHEN T2.ID = 5 THEN -1 ELSE 1 END * NB) pointsFid
					FROM TB19_FID_POINTS T1, TB18_FID_TYPE T2
					WHERE ID_MEMBER = ?0?
					AND ID_FID_TYPE = T2.ID
					) p2
				ORDER BY IFNULL(SEUIL,0) desc
				LIMIT 1
				";
				break;
				
			case 38:
				$req[0] = "select ID , Type
				from TB21_DOT_TYPE
				WHERE DELETED = 'N'";
				break;
				
			case 39:
				$req[0] = "select ID , Description
				from TB22_DOT_DESC
				WHERE DELETED = 'N'";
				break;
		}
		
		return $req;
	}
	
	function insert($numReq, $franchise)
	{
		switch ($numReq)
		{
			case 1:
				$req[0] = "INSERT INTO TB03_MEMBER (NAME, FIRSTNAME, BIRTH_DATE, ADRESS, ZIP_CODE, CITY, PHONE, EMAIL, KOSMOS
				, PICTURE, CLUB, CREATION_DATE, DELETED, ID_SPONSOR, POKERSPHERE_ID, PLAQUE_IMMATRICULATION)
				VALUES (UPPER('?0?'),CONCAT(UPPER(LEFT('?1?',1)),LOWER(SUBSTRING('?1?',2))),str_to_date('?3?','%d/%m/%Y'),'?4?','?5?','?6?','?7?','?8?','?9?','','?10?',NOW(), 'N', 
				CASE WHEN ?14? = -1 THEN NULL ELSE ?14? END, ?15?, '?17?')";
				
				$req[1] = "INSERT INTO TB04_ADHESION (ID_MEMBER, VALUE, OPERATION_DATE, ENABLE_DATE, DISABLE_DATE)
				VALUES (?id?,'?11?',NOW(),CURDATE(),str_to_date('?12?','%d/%m/%Y'))";
				
				$req[2] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON)
				VALUES (?id?,?9?,NOW(),'1','4')";
				
				$req[3] = "INSERT INTO TB13_MEMBER_CARD (ID_MEMBER, CARD_NUMBER)
				VALUES (?id?,'?13?')";
				
				$req[4] = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT, ID_REF_OPERATION)
				VALUES (?id?,NOW(),0,(SELECT (SELECT RATIO * ?16? FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = ?16?) * VALUE FROM TB18_FID_TYPE WHERE ID = 0),'Fidelite abonnement',?id2?)";
				
				$req[5] = "INSERT INTO TB20_RECAP (ID_MEMBER, NAME, FIRSTNAME, CREATION_DATE, NB_TOURNAMENT_PLAYED, NB_WIN, NB_ITM, NB_KOSMOS_IN_BUY_IN, NB_KOSMOS_WIN_IN_TOURNAMENT, NB_FID_POINTS)
				VALUES (?id?,UPPER('?0?'),CONCAT(UPPER(LEFT('?1?',1)),LOWER(SUBSTRING('?1?',2))),NOW(),0,0,0,0,0,IFNULL((SELECT (SELECT RATIO * ?16? FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = ?16?) * VALUE FROM TB18_FID_TYPE WHERE ID = 0),0))";
				
				$req[6] = "INSERT INTO TB09_MEMBER_ROOM (ID_MEMBER, ID_ROOM, PSEUDO, PUBLIC)
				VALUES (?id?, 8, '?2?', 0)
				ON DUPLICATE KEY UPDATE PSEUDO = '?2?'";
				break;
			
			case 2:
				$req[0] = "INSERT INTO TB04_ADHESION (ID_MEMBER, VALUE, OPERATION_DATE, ENABLE_DATE, DISABLE_DATE, PRIX)
				VALUES (?0?,'?1?',NOW(),str_to_date('?2?','%d/%m/%Y'),str_to_date('?3?','%d/%m/%Y'), ?4?)";
				
				$req[1] = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT, ID_REF_OPERATION)
				VALUES (?0?,NOW(),0,(SELECT (SELECT RATIO * ?5? FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = ?5?) * VALUE FROM TB18_FID_TYPE WHERE ID = 0),'Fidelite abonnement',?id?)";
				
				$req[2] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS + (SELECT (SELECT RATIO * ?5? FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = ?5?) * VALUE FROM TB18_FID_TYPE WHERE ID = 0)
				WHERE ID_MEMBER = ?0?";
				break;
			
			case 3:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON)
				VALUES (?0?,?1?,NOW(),'0',?2?)";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS - ?1?
				WHERE ID = ?0?";
				break;
			
			case 4:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON)
				VALUES (?0?,?1?,NOW(),'1',?2?)";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + ?1?
				WHERE ID = ?0?";
				break;
			
			case 5:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON,COMMENT)
				VALUES (?0?,?1?,NOW(),'0',3,'?2?')";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS - ?1?
				WHERE ID = ?0?";
				break;
			
			case 6:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON,COMMENT)
				VALUES (?0?,?1?,str_to_date('?2?','%d/%m/%Y %H:%i'),'?3?','?4?','?5?')";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + CASE WHEN '?3?' = '1' THEN ?1? ELSE -?1? END
				WHERE ID = ?0?";
				break;
				
			case 7:
				$req[0] = "INSERT INTO TB06_TOURNAMENT (NAME, BEGIN_DATE, END_REGISTRATION_DATE, BUY_IN, NB_PLAYERS, VARIANTE, VALIDATED, COMMENT, PRIVATE, TYPE, POKERSPHERE_ID, ATTACHED_TOURNAMENT)
				VALUES ('?0?',str_to_date('?1?','%d/%m/%Y %H:%i'),str_to_date('?2?','%d/%m/%Y %H:%i'),?3?,?4?,?5?,0,'?6?',?7?,?8?,?9?, ?10?)";
				break;
				
			case 8:
				$req[0] = "INSERT INTO TB07_TOURNAMENT_REGISTRATION (ID_TOURNAMENT, ID_MEMBER, REGISTRATION_DATE, IS_LATE)
				VALUES (?0?,?1?,NOW(), ?2?)";
				break;
			
			case 9:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				SELECT tr.ID_MEMBER, t.BUY_IN, NOW(), '0', t.TYPE, CONCAT(CASE t.TYPE WHEN 0 THEN 'Inscription au tournoi ' WHEN 1 THEN 'Inscription au Sit & Go ' END, t.NAME, ' du ', t.BEGIN_DATE)
				FROM TB07_TOURNAMENT_REGISTRATION tr,
				TB06_TOURNAMENT t
				WHERE tr.ID_TOURNAMENT = t.ID
				AND t.ID = ?0?";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS - (SELECT BUY_IN FROM TB06_TOURNAMENT WHERE ID = ?0?)
				WHERE ID IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				
				$req[2] = "UPDATE TB06_TOURNAMENT SET VALIDATED = 1
				WHERE ID = ?0?";
				
				$req[3] = "UPDATE TB07_TOURNAMENT_REGISTRATION T7
				SET ID_BUY_IN = (SELECT T5.ID FROM TB05_KOSMOS T5, TB06_TOURNAMENT T6
					WHERE T5.COMMENT = CONCAT(CASE T6.TYPE WHEN 0 THEN 'Inscription au tournoi ' WHEN 1 THEN 'Inscription au Sit & Go ' END, T6.NAME, ' du ', T6.BEGIN_DATE)
					AND T5.ID_MEMBER = T7.ID_MEMBER
					AND T6.ID = ?0?)
				WHERE ID_TOURNAMENT = ?0?";
				
				$req[4] = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT, ID_REF_OPERATION)
				(SELECT tr.ID_MEMBER, NOW(), 1, (SELECT VALUE FROM TB18_FID_TYPE WHERE ID = 1), CONCAT(CASE t.TYPE WHEN 0 THEN 'Inscription au tournoi ' WHEN 1 THEN 'Inscription au Sit & Go ' END, t.NAME, ' du ', t.BEGIN_DATE)
				, ?0?
				FROM TB07_TOURNAMENT_REGISTRATION tr,
				TB06_TOURNAMENT t
				WHERE tr.ID_TOURNAMENT = t.ID
				AND t.ID = ?0?)";
				
				$req[5] = "UPDATE TB20_RECAP T20
				SET NB_TOURNAMENT_PLAYED = NB_TOURNAMENT_PLAYED + 1
				WHERE ID_MEMBER IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				
				$req[6] = "UPDATE TB20_RECAP T20
				SET NB_KOSMOS_IN_BUY_IN = NB_KOSMOS_IN_BUY_IN + (SELECT BUY_IN 
					FROM TB06_TOURNAMENT
					WHERE ID = ?0?)
				WHERE ID_MEMBER IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				
				$req[7] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS + (SELECT VALUE FROM TB18_FID_TYPE WHERE ID = 1)
				WHERE ID_MEMBER IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				break;
			
			case 10:
				/*$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				SELECT tr.ID_MEMBER, t.BUY_IN, NOW(), '1', '0', CONCAT('Annulation du tournoi ', t.NAME, ' du ', t.BEGIN_DATE)
				FROM TB07_TOURNAMENT_REGISTRATION tr,
				TB06_TOURNAMENT t
				WHERE tr.ID_TOURNAMENT = t.ID
				AND t.ID = ?0?";*/
				
				$req[0] = "DELETE FROM TB05_KOSMOS 
				WHERE COMMENT = (SELECT CONCAT(CASE t.TYPE WHEN 0 THEN 'Inscription au tournoi ' WHEN 1 THEN 'Inscription au Sit & Go ' END, t.NAME, ' du ', t.BEGIN_DATE)
				FROM TB06_TOURNAMENT t
				WHERE t.ID = ?0?)";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + (SELECT BUY_IN FROM TB06_TOURNAMENT WHERE ID = ?0?)
				WHERE ID IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				
				$req[2] = "UPDATE TB06_TOURNAMENT SET VALIDATED = 0
				WHERE ID = ?0?";
				
				$req[3] = "UPDATE TB07_TOURNAMENT_REGISTRATION T7
				SET ID_BUY_IN = NULL
				WHERE ID_TOURNAMENT = ?0?";
				
				$req[4] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS - (SELECT NB FROM TB19_FID_POINTS WHERE ID_REF_OPERATION = ?0? AND ID_FID_TYPE = 1 LIMIT 1)
				WHERE ID_MEMBER IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				
				$req[5] = "DELETE FROM TB19_FID_POINTS WHERE ID_REF_OPERATION = ?0? AND ID_FID_TYPE = 1";
				
				$req[6] = "UPDATE TB20_RECAP T20
				SET NB_TOURNAMENT_PLAYED = NB_TOURNAMENT_PLAYED - 1
				WHERE ID_MEMBER IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				
				$req[7] = "UPDATE TB20_RECAP T20
				SET NB_KOSMOS_IN_BUY_IN = NB_KOSMOS_IN_BUY_IN - (SELECT BUY_IN 
					FROM TB06_TOURNAMENT
					WHERE ID = ?0?)
				WHERE ID_MEMBER IN (SELECT ID_MEMBER FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?)";
				break;
			
			case 11:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				SELECT a.ID_MEMBER, 1000, NOW(), '1', '4', 
				CONCAT('Bonus mensuel adhesion ', MONTHNAME(str_to_date('?0?','%d/%m/%Y')), ' ', YEAR(str_to_date('?0?','%d/%m/%Y')))
				from TB04_ADHESION a
				WHERE a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER
																	AND a2.ENABLE_DATE < str_to_date('?0?','%d/%m/%Y'))
				AND  str_to_date('?0?','%d/%m/%Y') < a.DISABLE_DATE";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + 1000
				WHERE ID IN (SELECT a.ID_MEMBER
							from TB04_ADHESION a
							WHERE a.DISABLE_DATE = (select max(a2.DISABLE_DATE) from TB04_ADHESION a2 where a2.ID_MEMBER = a.ID_MEMBER
																				AND a2.ENABLE_DATE < str_to_date('?0?','%d/%m/%Y'))
							AND  str_to_date('?0?','%d/%m/%Y') < a.DISABLE_DATE)";
				break;
				
			case 12:
				$req[0] = "INSERT INTO TB12_TOURNAMENT_TEMPLATE (NAME, BUY_IN, NB_PLAYERS, VARIANTE, COMMENT, POKERSPHERE_ID)
				VALUES ('?0?',?1?,?2?,?3?,'?4?',?5?)";
				break;
			
			case 13:
				$req[0] = "INSERT INTO TB13_MEMBER_CARD (ID_MEMBER, CARD_NUMBER)
				VALUES (?0?,'?13?')
				ON DUPLICATE KEY UPDATE CARD_NUMBER = '?13?'";
				break;
				
			case 14:
				$req[0] = "INSERT INTO TB13_MEMBER_CARD (ID_MEMBER, CARD_NUMBER)
				VALUES (?0?,'?1?')
				ON DUPLICATE KEY UPDATE CARD_NUMBER = '?1?'";
				break;
			
			case 15:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				VALUES (?id?,IFNULL((SELECT SUM(GODSON_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 0 AND TYPE_GODSON_GAIN = 0),0) ,NOW(),'1','2',
				'Gain immediat parrain')";
				
				$req[1] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				VALUES (?id?,IFNULL((SELECT SUM(GODSON_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 0 AND TYPE_GODSON_GAIN = 2),0) ,NOW(),'1','3',
				'Bonus immediat parrain')";
				
				$req[2] = "UPDATE TB03_MEMBER 
				SET KOSMOS = KOSMOS + (SELECT SUM(GODSON_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 0 AND TYPE_GODSON_GAIN IN (0,2))
				WHERE ID = ?id?";
				
				$req[3] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				VALUES (?14?,IFNULL((SELECT SUM(SPONSOR_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 0 AND TYPE_SPONSOR_GAIN = 0),0) ,NOW(),'1','2',
				'Gain immediat filleul')";
				
				$req[4] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				VALUES (?14?,IFNULL((SELECT SUM(SPONSOR_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 0 AND TYPE_SPONSOR_GAIN = 2),0) ,NOW(),'1','3',
				'Bonus immediat filleul')";
				
				$req[5] = "UPDATE TB03_MEMBER 
				SET KOSMOS = KOSMOS + (SELECT SUM(SPONSOR_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 0 AND TYPE_SPONSOR_GAIN = 0)
				WHERE ID = ?14?";
				
				break;
				
			case 16:
				$req[0] = "SELECT ID FROM TB03_MEMBER WHERE EMAIL = '?1?' AND DELETED = 'N'";
				
				$req[1] = "INSERT INTO TB07_TOURNAMENT_REGISTRATION (ID_TOURNAMENT, ID_MEMBER, REGISTRATION_DATE, IS_LATE)
				VALUES (?0?, ?id? ,NOW(), 0)";
				
				$req[2] = "SELECT ID FROM TB03_MEMBER WHERE UPPER(NAME) = UPPER('?2?') AND UPPER(FIRSTNAME) = UPPER('?3?') AND DELETED = 'N'";
				
				$req[3] = "INSERT INTO TB03_MEMBER (NAME, FIRSTNAME, PSEUDO, BIRTH_DATE, ADRESS, ZIP_CODE, CITY, PHONE, EMAIL, KOSMOS
				, PICTURE, CLUB, CREATION_DATE, DELETED, ID_SPONSOR, POKERSPHERE_ID)
				VALUES (UPPER('?2?'), CONCAT(UCASE(LEFT('?3?', 1)),LCASE(SUBSTRING('?3?', 2))),NULL,NULL,NULL,NULL,NULL,NULL,'?1?',10000,'',NULL,NOW(), 'N', 
				NULL , ?franchise?)";
				
				$req[4] = "INSERT INTO TB04_ADHESION (ID_MEMBER, VALUE, OPERATION_DATE, ENABLE_DATE, DISABLE_DATE)
				VALUES (?id?,0,NOW(),CURDATE(),CURDATE()+1)";
				
				$req[5] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON)
				VALUES (?id?,10000,NOW(),'1','4')";
				
				$req[6] = "SELECT POKERSPHERE_ID FROM TB06_TOURNAMENT WHERE ID = ?0?";
				
				$req[7] = "INSERT INTO TB13_MEMBER_CARD (ID_MEMBER, CARD_NUMBER)
				VALUES (?id?,NULL)";
				
				break;
				
			case 17:
				$req[0] = "INSERT INTO TB03_MEMBER (NAME, FIRSTNAME, PSEUDO, BIRTH_DATE, ADRESS, ZIP_CODE, CITY, PHONE, EMAIL, KOSMOS
				, PICTURE, CLUB, CREATION_DATE, DELETED, ID_SPONSOR, POKERSPHERE_ID)
				VALUES (UPPER('?0?'),CONCAT(UCASE(LEFT('?1?', 1)),LCASE(SUBSTRING('?1?', 2))),NULL,NULL,NULL,NULL,NULL,NULL,'?2?',10000,'',NULL,NOW(), 'N', 
				NULL , ?3?)";
				
				$req[1] = "INSERT INTO TB04_ADHESION (ID_MEMBER, VALUE, OPERATION_DATE, ENABLE_DATE, DISABLE_DATE)
				VALUES (?id?,0,NOW(),CURDATE(),CURDATE()+1)";
				
				$req[2] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON)
				VALUES (?id?,10000,NOW(),'1','4')";
				
				$req[3] = "INSERT INTO TB13_MEMBER_CARD (ID_MEMBER, CARD_NUMBER)
				VALUES (?id?,NULL)";
				
				$req[4] = "INSERT INTO TB07_TOURNAMENT_REGISTRATION (ID_TOURNAMENT, ID_MEMBER, REGISTRATION_DATE, IS_LATE)
				VALUES (?4?, ?id? ,NOW(), 0)";
				break;
				
			case 18:
				$req[0] = "INSERT INTO TB07_TOURNAMENT_REGISTRATION (ID_TOURNAMENT, ID_MEMBER, REGISTRATION_DATE, IS_LATE)
				VALUES (?0?,?1?,NOW(), 0)";
				break;
				
			case 19:
				$req[0] = "INSERT INTO TB16_CLUB (CLUB, POKERSPHERE_ID)
				VALUES ('?0?',?1?)";
				break;
				
			case 20:
				$req[0] = "INSERT INTO TB17_FID_STATUT (STATUT, SEUIL)
				VALUES ('?0?',?1?)";
				break;
				
			case 21:
				$req[0] = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT)
				VALUES (?0?,str_to_date('?1?','%d/%m/%Y %H:%i'),?2?,?3?,'?4?')";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS + ?3?
				WHERE ID_MEMBER = ?0?";
				
				break;
				
			case 22:
				$req[0] = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT, ID_REF_OPERATION)
				VALUES (?0?,NOW(),2,(SELECT ?1? * VALUE FROM TB18_FID_TYPE WHERE ID = 2),'Chargement Euros','')";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS + (SELECT ?1? * VALUE FROM TB18_FID_TYPE WHERE ID = 2)
				WHERE ID_MEMBER = ?0?";
				break;
				
			case 23:
				$req[0] = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT, ID_REF_OPERATION)
				VALUES (?1?,NOW(),4,?3?,'Gains Tournoi',?0?)";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS + ?3?
				WHERE ID_MEMBER = ?1?";
				break;
				
			case 24:
				$req[0] = "INSERT INTO TB21_DOT_TYPE (TYPE)
				VALUES ('?0?')";
				break;
				
			case 25:
				$req[0] = "INSERT INTO TB22_DOT_DESC (DESCRIPTION)
				VALUES ('?0?')";
				break;
				
			case 26:
				$req[0] = "INSERT INTO TB23_TOURNAMENT_DOTATION (ID_TOURNAMENT, ID_MEMBER, ID_TYPE_DOTATION, VALUE, ID_DESCRIPTION_DOTATION)
				VALUES (?0?,?1?,(SELECT ID FROM TB21_DOT_TYPE WHERE TYPE = '?a?' AND DELETED = 'N'), ?b?
				, (SELECT ID FROM TB22_DOT_DESC WHERE DESCRIPTION = '?c?' AND DELETED = 'N') )";
				break;
		}
		return $req;
	}
	
	function update($numReq, $franchise)
	{
		switch ($numReq)
		{
			case 1:
				$req[0] = "UPDATE TB03_MEMBER SET NAME = '?1?', FIRSTNAME = '?2?'
				, BIRTH_DATE = str_to_date('?4?','%d/%m/%Y'), ADRESS = '?5?', ZIP_CODE = '?6?', CITY = '?7?'
				, PHONE = '?8?', EMAIL = '?9?', KOSMOS = '?10?', CLUB = '?11?', ID_SPONSOR = ?12?, PLAQUE_IMMATRICULATION = '?13?'
				WHERE ID = ?0?";
				
				$req[1] = "INSERT INTO TB09_MEMBER_ROOM (ID_MEMBER, ID_ROOM, PSEUDO, PUBLIC)
				VALUES (?0?, 8, '?3?', 0)
				ON DUPLICATE KEY UPDATE PSEUDO = '?3?'";
				break;
			
			case 2:
				$req[0] = "UPDATE TB04_ADHESION SET VALUE = '?1?', OPERATION_DATE = str_to_date('?2?','%d/%m/%Y %H:%i')
				,ENABLE_DATE = str_to_date('?3?','%d/%m/%Y') , DISABLE_DATE = str_to_date('?4?','%d/%m/%Y'), PRIX = ?5?
				WHERE ID = ?0?";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS - (SELECT NB FROM TB19_FID_POINTS WHERE ID_REF_OPERATION = ?0? AND ID_FID_TYPE = 0)
					+ (SELECT (SELECT RATIO * ?6? FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = ?6?) * VALUE FROM TB18_FID_TYPE WHERE ID = 0)
				WHERE ID_MEMBER = (SELECT ID_MEMBER FROM TB04_ADHESION WHERE ID = ?0?)";
				
				$req[2] = "UPDATE TB19_FID_POINTS SET EVENT_DATE = NOW(), 
				NB = (SELECT (SELECT RATIO * ?6? FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = ?6?) * VALUE FROM TB18_FID_TYPE WHERE ID = 0)
				WHERE ID_REF_OPERATION = ?0?
				AND ID_FID_TYPE = 0";
				break;
			
			case 3:
				$req[0] = "UPDATE TB05_KOSMOS SET VALUE = '?1?', OPERATION_DATE = str_to_date('?2?','%d/%m/%Y %H:%i')
				, CREDIT = '?3?', REASON = '?4?', COMMENT = '?5?'
				WHERE ID = ?0?";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + ?7?
				WHERE ID = ?6?";
				break;
			
			case 4:
				$req[0] = "UPDATE TB06_TOURNAMENT SET NAME = '?1?', BEGIN_DATE = str_to_date('?2?','%d/%m/%Y %H:%i')
				, END_REGISTRATION_DATE = str_to_date('?3?','%d/%m/%Y %H:%i')
				, BUY_IN = '?4?', NB_PLAYERS = '?5?', VARIANTE = '?6?', COMMENT = '?7?', PRIVATE = ?8?, ATTACHED_TOURNAMENT = ?9?
				WHERE ID = ?0?";
				break;
			
			case 5:
				$req[0] = "UPDATE TB07_TOURNAMENT_REGISTRATION SET IS_ARRIVED = ?2?
				WHERE ID_TOURNAMENT = ?0?
				AND ID_MEMBER = ?1?";
				break;
			
			case 6:
				$req[0] = "UPDATE TB07_TOURNAMENT_REGISTRATION 
				SET POSITION = ?2?
				WHERE ID_TOURNAMENT = ?0?
				AND ID_MEMBER = ?1?";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_WIN = NB_WIN + CASE ?2? WHEN 1 THEN 1 ELSE 0 END
				WHERE ID_MEMBER = ?1?";
				break;
				
			case 7:
				$req[0] = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
				SELECT ?1?, ?4?, NOW(), '1', t.TYPE, CONCAT(CASE t.TYPE WHEN 0 THEN 'Gain du tournoi ' WHEN 1 THEN 'Gain du Sit & Go ' END , t.NAME, ' du ', t.BEGIN_DATE)
				FROM TB06_TOURNAMENT t
				WHERE t.ID = ?0?";
				
				$req[1] = "UPDATE TB07_TOURNAMENT_REGISTRATION 
				SET POSITION = ?2?,
				ID_GAIN = ?id?
				WHERE ID_TOURNAMENT = ?0?
				AND ID_MEMBER = ?1?";
				
				$req[2] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + ?4?
				WHERE ID = ?1?";
				
				$req[3] = "UPDATE TB20_RECAP T20
				SET NB_WIN = NB_WIN + CASE ?2? WHEN 1 THEN 1 ELSE 0 END
				WHERE ID_MEMBER = ?1?";
				
				$req[4] = "UPDATE TB20_RECAP T20
				SET NB_ITM = NB_ITM + 1
				WHERE ID_MEMBER = ?1?";
				
				$req[5] = "UPDATE TB20_RECAP T20
				SET NB_KOSMOS_WIN_IN_TOURNAMENT = NB_KOSMOS_WIN_IN_TOURNAMENT + ?4?
				WHERE ID_MEMBER = ?1?";
				break;
				
			case 8:
				$req[0] = "UPDATE TB03_MEMBER 
				SET KOSMOS = KOSMOS - IFNULL((SELECT VALUE 
					FROM TB05_KOSMOS,TB07_TOURNAMENT_REGISTRATION T7
					WHERE ID_TOURNAMENT = ?0?
					AND T7.ID_MEMBER = ?1?
					AND ID_GAIN = ID),0)
				WHERE ID = ?1?";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_ITM = NB_ITM - (SELECT count('x') 
					FROM TB05_KOSMOS,TB07_TOURNAMENT_REGISTRATION T7
					WHERE ID_TOURNAMENT = ?0?
					AND T7.ID_MEMBER = ?1?
					AND ID_GAIN = ID)
				WHERE ID_MEMBER = ?1?";
				
				$req[2] = "UPDATE TB20_RECAP T20
				SET NB_KOSMOS_WIN_IN_TOURNAMENT = NB_KOSMOS_WIN_IN_TOURNAMENT - IFNULL((SELECT VALUE 
					FROM TB05_KOSMOS,TB07_TOURNAMENT_REGISTRATION T7
					WHERE ID_TOURNAMENT = ?0?
					AND T7.ID_MEMBER = ?1?
					AND ID_GAIN = ID),0)
				WHERE ID_MEMBER = ?1?";
				
				$req[3] = "DELETE FROM TB05_KOSMOS
				WHERE ID = (SELECT ID_GAIN FROM TB07_TOURNAMENT_REGISTRATION T7
					WHERE ID_TOURNAMENT = ?0?
					AND T7.ID_MEMBER = ?1?)";
				
				$req[4] = "UPDATE TB20_RECAP T20
				SET NB_WIN = NB_WIN - IFNULL((SELECT 1 FROM TB07_TOURNAMENT_REGISTRATION 
										WHERE ID_TOURNAMENT = ?0?
										AND ID_MEMBER = ?1?
										AND POSITION = 1),0)
				WHERE ID_MEMBER = ?1?";
				
				$req[5] = "UPDATE TB07_TOURNAMENT_REGISTRATION 
				SET POSITION = NULL,
				ID_GAIN = NULL
				WHERE ID_TOURNAMENT = ?0?
				AND ID_MEMBER = ?1?";
				
				$req[6] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS - IFNULL((SELECT NB FROM TB19_FID_POINTS WHERE ID_FID_TYPE = 4 AND ID_REF_OPERATION = ?0? AND ID_MEMBER = ?1?),0)
				WHERE ID_MEMBER = ?1?";
				
				$req[7] = "DELETE FROM TB19_FID_POINTS
				WHERE ID_FID_TYPE = 4
				AND ID_REF_OPERATION = ?0?
				AND ID_MEMBER = ?1?";
				
				$req[8] = "DELETE FROM TB23_TOURNAMENT_DOTATION
				WHERE ID_TOURNAMENT = ?0?
				AND ID_MEMBER = ?1?";
				
				break;
				
			case 9:
				$req[0] = "UPDATE TB12_TOURNAMENT_TEMPLATE SET NAME = '?1?'
				, BUY_IN = '?2?', NB_PLAYERS = '?3?', VARIANTE = '?4?', COMMENT = '?5?'
				WHERE ID = ?0?";
				break;
				
			case 10:
				$req[0] = "UPDATE TB15_GAIN_SPONSOR SET LAUNCHING = ?1?, TYPE_GODSON_GAIN = ?2?
				,GODSON_GAIN = ?3?, TYPE_SPONSOR_GAIN = ?4?, SPONSOR_GAIN = ?5?, NB_MONTH_FIRST_MEMBERSHIP = '?6?'
				WHERE ID = ?0?";
				break;
				
			case 11:
				$req[0] = "UPDATE TB06_TOURNAMENT SET SENT_TO_RANKINGHERO = 1
				WHERE ID = ?0?";
				break;
				
			case 12:
				$req[0] = "UPDATE TB03_MEMBER SET POKERSPHERE_ID = ?1?
				WHERE ID = ?0?";
				
				$req[1] = "UPDATE TB01_USERS SET POKERSPHERE_ID = ?1?
				WHERE ID_MEMBER = ?0?";
				break;
				
			case 13:
				$req[0] = "SELECT MAX(POSITION) + 1 
				FROM TB07_TOURNAMENT_REGISTRATION 
				WHERE ID_TOURNAMENT = ?0?";
				
				$req[1] = "UPDATE TB07_TOURNAMENT_REGISTRATION 
				SET POSITION = ?id?
				WHERE ID_TOURNAMENT = ?0? 
				AND POSITION IS NULL";
				break;
				
			case 14:
				$req[0] = "UPDATE TB16_CLUB SET CLUB = '?1?'
				WHERE CLUB = '?0?'
				AND POKERSPHERE_ID = ?2?";
				break;
				
			case 15:
				$req[0] = "UPDATE TB17_FID_STATUT SET STATUT = '?1?'
				, SEUIL = '?2?'
				WHERE ID = ?0?";
				break;
				
			case 16:
				$req[0] = "DELETE FROM TB18_FID_TYPE WHERE ID < 5";
				
				$req[1] = "INSERT INTO TB18_FID_TYPE (ID, NAME, RATIO, VALUE, NB_MONTH)
				VALUES 
				(?0?,'?1?',?2?,?3?,?4?),
				(?5?,'?6?',?7?,?8?,?9?),
				(?10?,'?11?',?12?,?13?,?14?),
				(?15?,'?16?',?17?,?18?,?19?),
				(?20?,'?21?',?22?,?23?,?24?)";
				
				break;
				
			case 17:
				$req[0] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS - (SELECT NB FROM TB19_FID_POINTS WHERE ID = ?0?) + ?1?
				WHERE ID_MEMBER = (SELECT ID_MEMBER FROM TB19_FID_POINTS WHERE ID = ?0?)";
				
				$req[1] = "UPDATE TB19_FID_POINTS SET NB = '?1?', EVENT_DATE = str_to_date('?2?','%d/%m/%Y %H:%i')
				, ID_FID_TYPE = '?3?', COMMENT = '?4?'
				WHERE ID = ?0?";
				
				break;
				
			case 18:
				$req[0] = "UPDATE TB21_DOT_TYPE SET TYPE = '?1?'
				WHERE TYPE = '?0?'";
				break;
				
			case 19:
				$req[0] = "UPDATE TB22_DOT_DESC SET DESCRIPTION = '?1?'
				WHERE DESCRIPTION = '?0?'";
				break;
		}
		return $req;
	}
	
	function delete($numReq, $franchise)
	{
		switch ($numReq)
		{
			case 1:
				//$req[0] = "DELETE FROM TB03_MEMBER WHERE ID = ?0?";
				$req[0] = "UPDATE TB03_MEMBER SET DELETED = 'Y' WHERE ID = ?0?";
				break;
			
			case 2:
				$req[0] = "DELETE FROM TB04_ADHESION WHERE ID = ?0?";
				
				$req[1] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS - (SELECT NB FROM TB19_FID_POINTS WHERE ID_REF_OPERATION = ?0? AND ID_FID_TYPE = 0)
				WHERE ID_MEMBER = SELECT ID_MEMBER FROM TB04_ADHESION WHERE ID = ?0?";
				
				$req[2] = "DELETE FROM TB19_FID_POINTS WHERE ID_REF_OPERATION = ?0? AND ID_FID_TYPE = 0";
				break;
			
			case 3:
				$req[0] = "DELETE FROM TB05_KOSMOS WHERE ID = ?0?";
				
				$req[1] = "UPDATE TB03_MEMBER SET KOSMOS = KOSMOS + ?2?
				WHERE ID = ?1?";
				break;
				
			case 4:
				$req[0] = "DELETE FROM TB06_TOURNAMENT WHERE ID = ?0?";
				
				$req[1] = "DELETE FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0?";
				break;
			
			case 5:
				$req[0] = "DELETE FROM TB07_TOURNAMENT_REGISTRATION WHERE ID_TOURNAMENT = ?0? AND ID_MEMBER = ?1?";
				break;
				
			case 6:
				$req[0] = "DELETE FROM TB12_TOURNAMENT_TEMPLATE WHERE ID = ?0?";
				break;
				
			case 7:
				$req[0] = "DELETE FROM TB16_CLUB WHERE CLUB = '?0?' AND POKERSPHERE_ID = ?1?";
				break;
				
			case 8:
				$req[0] = "DELETE FROM TB17_FID_STATUT WHERE ID = ?0?";
				break;
				
			case 9:
				$req[0] = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS - (SELECT NB FROM TB19_FID_POINTS WHERE ID = ?0?)
				WHERE ID_MEMBER = (SELECT ID_MEMBER FROM TB19_FID_POINTS WHERE ID = ?0?)";
				
				$req[1] = "DELETE FROM TB19_FID_POINTS WHERE ID = ?0?";
				break;
				
			case 10:
				$req[0] = "UPDATE TB21_DOT_TYPE SET DELETED = 'Y'
				WHERE TYPE = '?0?'";
				break;
				
			case 11:
				$req[0] = "UPDATE TB22_DOT_DESC SET DELETED = 'Y'
				WHERE DESCRIPTION = '?0?'";
				break;
		}
		return $req;
	}
	
	function hexToStr($hex)
	{
		$string = "";
		for ($i=0; $i < strlen($hex)-1; $i+=2)
		{
			$string .= chr(hexdec($hex[$i].$hex[$i+1]));
		}
		return $string;
	}
	
	function strToHex($string)
	{
		$hex = "";
		for ($i=0; $i < strlen($string); $i++)
		{
			//$hex .= dechex(ord($string[$i]));
			$hex .= str_pad(dechex(ord($string[$i])),2,"0",STR_PAD_LEFT);
		}
		return $hex;
	}
	
	function savePhoto($id, $data)
	{
		chdir('photos');
		$filename = $id.'.jpg';
		if (file_exists($filename))
			unlink($filename);
		$file = fopen($filename,'ab');
		fputs($file,hexToStr($data));
		fclose($file);
	}
	
	function getPhoto($id)
	{
		$filename = 'photos/'.$id.'.jpg';
		if (file_exists($filename))
			$res = file_get_contents($filename);
		else
			return "NO_PHOTO";
		return strToHex($res);
	}
	
	function getInfosPhoto($id)
	{
		$req = select(37, -1)[0];
		$req = str_replace('?0?',$id,$req);
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$statut = $result[0];
			//echo '<SEP>';
			//echo $statut;
			$points = $result[1];
			//echo '<SEP>';
			//echo $points;
			$pointsFid = $result[2];
			echo '<SEP>';
			echo $pointsFid;
		}
		return;
	}
	
	function arrive($cardNumber, $typeLog, $franchise)
	{
		//Recuperation du membre correspondant.
		$req = "SELECT ID, NAME, FIRSTNAME, DATE_FORMAT(SYSDATE(),'%d/%m/%Y à %Hh%i')
				FROM TB03_MEMBER, TB13_MEMBER_CARD
				WHERE ID_MEMBER = ID
				AND CARD_NUMBER = '$cardNumber'";
		
		$donnees = mysql_query($req);
		
		if (mysql_num_rows($donnees) == 0)
		{
			echo '<LOG_ERROR>';
			echo "Le numero de carte '$cardNumber' n'est pas attribue.\nVoulez vous l'attribuer ?";
			return;
		}
		
		//Verification que le membre n'est pas dejà passe
		$msg = 'est arrive pour un tournoi le';
		if ($typeLog == 1)
		{
			$msg = 'est arrive pour une partie libre le';
		}
		
		$req = "SELECT 'x'
				FROM TB14_LOG TB14, TB13_MEMBER_CARD TB13
				WHERE TB14.ID_MEMBER = TB13.ID_MEMBER
				AND CARD_NUMBER = '$cardNumber'
				AND EVENT like '%$msg%'
				AND EVENT_DATE BETWEEN DATE_SUB(SYSDATE(), INTERVAL 12 HOUR) AND DATE_ADD(SYSDATE(), INTERVAL 12 HOUR)";
		
		$data_verif = mysql_query($req);
		
		if (mysql_num_rows($data_verif) != 0)
		{
			echo '<LOG_ERROR>';
			echo "Ce membre est dejà passe aujourd'hui.";
			return;
		}
		
		$memberId = "";
		$member = "";
		
		while ($result = mysql_fetch_row($donnees))
		{
			$memberId = $result[0];
			$member = $result[2] . ' ' . $result[1];
			$log = $member . " " . $msg . " " . $result[3] . '.';
			
			$req = "INSERT INTO TB14_LOG (ID_MEMBER, EVENT, EVENT_DATE)
					VALUES ($memberId,'$member $msg $result[3].',SYSDATE())";
			
			$insert_data = mysql_query($req);
		}
		
		if ($typeLog == 2)
		{
			//Verification s'il y a un tournoi
			$req = "SELECT ID, NAME
					FROM TB06_TOURNAMENT
					WHERE BEGIN_DATE BETWEEN DATE_SUB(SYSDATE(), INTERVAL 12 HOUR) AND DATE_ADD(SYSDATE(), INTERVAL 12 HOUR)
					AND POKERSPHERE_ID = $franchise";
			
			$donnees = mysql_query($req);

			$idTournament = "";
			
			if (mysql_num_rows($donnees) == 0)
			{
				$log .= "<LOG_SEP>Pas de tournoi aujourd'hui.";
			}
			else
			{
				while ($result = mysql_fetch_row($donnees))
				{
					$idTournament = $result[0];
					$log .= "<LOG_SEP>$member s'est inscrit au tournoi \"$result[1]\".";
					
					$req = "INSERT INTO TB14_LOG (ID_MEMBER, EVENT, EVENT_DATE)
						VALUES ($memberId,'$member s\'est inscrit au tournoi \"$result[1]\".',SYSDATE())";
				
					$insert_data = mysql_query($req);
					
					$req = "INSERT INTO TB07_TOURNAMENT_REGISTRATION (ID_TOURNAMENT, ID_MEMBER, REGISTRATION_DATE, IS_LATE, IS_ARRIVED)
					VALUES ($idTournament,$memberId,NOW(), 0, 1)
					ON DUPLICATE KEY UPDATE IS_ARRIVED = 1";
					
					$insert_data = mysql_query($req);
				}
			}
		}
		
		//Verification abonnement
		$req = "SELECT CASE WHEN SYSDATE() <= DATE_SUB(DISABLE_DATE, INTERVAL 7 DAY) THEN 1 ELSE 0 END
				FROM TB04_ADHESION
				WHERE ID_MEMBER = $memberId
				AND SYSDATE() >= ENABLE_DATE
				AND SYSDATE() <= DISABLE_DATE";
				
		$donnees = mysql_query($req);
		
		if (mysql_num_rows($donnees) == 0)
		{
			$log .= "<LOG_ALERT>L'abonnement de $member a expire.";
		}
		else
		{
			while ($result = mysql_fetch_row($donnees))
			{
				if ($result[0] == 0)
				{
					$log .= "<LOG_ALERT>L'abonnement de $member expire dans moins d'une semaine.";
				}
			}
		}
		
		echo '<LOG>';
		echo $log;
	}
	
	function needSponsorBonus($member, $typeAbonnement, $nbMonth)
	{
		// echo "calcul bonus";
		$count = 0;
		$sponsor_id = 0;
		
		//Verification que c'est la première adhesion d'un membre.
		$req = "SELECT count('x'), ID_SPONSOR
				FROM TB04_ADHESION, TB03_MEMBER T3
				WHERE ID_MEMBER = $member
				AND ID_MEMBER = T3.ID
				AND VALUE BETWEEN 1 AND 4
				GROUP BY ID_SPONSOR";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$count = $result[0];
			$sponsor_id = $result[1];
		}
		
		if ($count == 1 && $sponsor_id > 0)
		{
			computeSponsorBonus($member, $sponsor_id, $typeAbonnement, $nbMonth );
		}
		// else
		// {
			// echo "pas de bonus";
		// }
		
	}
	
	function computeSponsorBonus($member, $sponsor_id, $typeAbonnement, $nbMonth)
	{
		//Prise en compte des points fidelites
		computeFidSponsor($sponsor_id, $member, $nbMonth);
		
		$nb_month = array(0,1,3,6,12);
		
		$nbKosmosGodson = 0;
		$nbKosmosBonusGodson = 0;
		$nbKosmosSponsor = 0;
		$nbKosmosBonusSponsor = 0;
		$nbDayGodson = 0;
		$nbDaySponsor = 0;
		
		//Recuperation du nombre de kosmos en gain du filleul
		$req = "SELECT SUM(GODSON_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 1 
					AND TYPE_GODSON_GAIN = 0 AND NB_MONTH_FIRST_MEMBERSHIP = $nb_month[$typeAbonnement]";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$nbKosmosGodson = $result[0];
		}
		
		//Recuperation du nombre de kosmos bonus du filleul
		$req = "SELECT SUM(GODSON_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 1 
					AND TYPE_GODSON_GAIN = 2 AND NB_MONTH_FIRST_MEMBERSHIP = $nb_month[$typeAbonnement]";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$nbKosmosBonusGodson = $result[0];
		}
		
		//Recuperation du nombre de kosmos en gain du parrain
		$req = "SELECT SUM(SPONSOR_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 1 
					AND TYPE_SPONSOR_GAIN = 0 AND NB_MONTH_FIRST_MEMBERSHIP = $nb_month[$typeAbonnement]";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$nbKosmosSponsor = $result[0];
		}
		
		//Recuperation du nombre de kosmos en gain du parrain
		$req = "SELECT SUM(SPONSOR_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 1 
					AND TYPE_SPONSOR_GAIN = 2 AND NB_MONTH_FIRST_MEMBERSHIP = $nb_month[$typeAbonnement]";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$nbKosmosBonusSponsor = $result[0];
		}
		
		//Recuperation du nombre de jour d'abonnement en gain du filleul
		$req = "SELECT SUM(GODSON_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 1 
					AND TYPE_GODSON_GAIN = 1 AND NB_MONTH_FIRST_MEMBERSHIP = $nb_month[$typeAbonnement]";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$nbDayGodson = $result[0];
		}
		
		//Recuperation du nombre de jour d'abonnement en gain du parrain
		$req = "SELECT SUM(SPONSOR_GAIN) FROM TB15_GAIN_SPONSOR WHERE LAUNCHING = 1 
					AND TYPE_SPONSOR_GAIN = 1 AND NB_MONTH_FIRST_MEMBERSHIP = $nb_month[$typeAbonnement]";
		
		$donnees = mysql_query($req);
		
		while ($result = mysql_fetch_row($donnees))
		{
			$nbDaySponsor = $result[0];
		}
			

		if ($nbKosmosGodson > 0)
		{
			// echo "filleul kosmos";
			$req = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
					VALUES ($member,$nbKosmosGodson,NOW(),'1','2','Gain parrain activation abonnement')";
			
			$data_verif = mysql_query($req);

			$req = "UPDATE TB03_MEMBER 
					SET KOSMOS = KOSMOS + $nbKosmosGodson
					WHERE ID = $member";
					
			$data_verif = mysql_query($req);
		}
		
		if ($nbKosmosBonusGodson > 0)
		{
			// echo "filleul kosmos";
			$req = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
					VALUES ($member,$nbKosmosBonusGodson,NOW(),'1','3','Bonus parrain activation abonnement')";
			
			$data_verif = mysql_query($req);

			$req = "UPDATE TB03_MEMBER 
					SET KOSMOS = KOSMOS + $nbKosmosBonusGodson
					WHERE ID = $member";
					
			$data_verif = mysql_query($req);
		}
		
		if ($nbKosmosSponsor > 0)
		{
			// echo "sponsor kosmos";
			$req = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
					VALUES ($sponsor_id,$nbKosmosSponsor,NOW(),'1','2','Gain filleul activation abonnement')";
			
			$data_verif = mysql_query($req);

			$req = "UPDATE TB03_MEMBER 
					SET KOSMOS = KOSMOS + $nbKosmosSponsor
					WHERE ID = $sponsor_id";
					
			$data_verif = mysql_query($req);
		}
		
		if ($nbKosmosBonusSponsor > 0)
		{
			// echo "sponsor kosmos";
			$req = "INSERT INTO TB05_KOSMOS (ID_MEMBER, VALUE, OPERATION_DATE, CREDIT, REASON, COMMENT)
					VALUES ($sponsor_id,$nbKosmosBonusSponsor,NOW(),'1','3','Bonus filleul activation abonnement')";
			
			$data_verif = mysql_query($req);

			$req = "UPDATE TB03_MEMBER 
					SET KOSMOS = KOSMOS + $nbKosmosBonusSponsor
					WHERE ID = $sponsor_id";
					
			$data_verif = mysql_query($req);
		}
		
		if ($nbDayGodson > 0)
		{
			// echo "filleul adhesion";
			//Verification de la date de fin de l'abonnement du Filleul
			$req = "SELECT CASE WHEN MAX(DISABLE_DATE) < SYSDATE() THEN DATE_FORMAT(SYSDATE(),'%d/%m/%Y') 
						ELSE DATE_FORMAT(DISABLE_DATE,'%d/%m/%Y') END
					FROM TB04_ADHESION
					WHERE ID_MEMBER = $member";
					
			$donnees = mysql_query($req);
			
			while ($result = mysql_fetch_row($donnees))
			{
				$endDate = $result[0];
			}
			
			$req = "INSERT INTO TB04_ADHESION (ID_MEMBER, VALUE, OPERATION_DATE, ENABLE_DATE, DISABLE_DATE)
					VALUES ($member,1,NOW(),DATE_ADD(str_to_date('$endDate','%d/%m/%Y'), INTERVAL 1 DAY)
							,DATE_ADD(str_to_date('$endDate','%d/%m/%Y')
							, INTERVAL $nbDayGodson+1 DAY))";
			
			$donnees = mysql_query($req);
		}
		
		if ($nbDaySponsor > 0)
		{
			// echo "sponsor adhesion";
			//Verification de la date de fin de l'abonnement du Parrain
			$req = "SELECT CASE WHEN MAX(DISABLE_DATE) < SYSDATE() THEN DATE_FORMAT(SYSDATE(),'%d/%m/%Y') 
						ELSE DATE_FORMAT(DISABLE_DATE,'%d/%m/%Y') END
					FROM TB04_ADHESION
					WHERE ID_MEMBER = $sponsor_id";
					
			$donnees = mysql_query($req);
			
			while ($result = mysql_fetch_row($donnees))
			{
				$endDate = $result[0];
			}
			
			$req = "INSERT INTO TB04_ADHESION (ID_MEMBER, VALUE, OPERATION_DATE, ENABLE_DATE, DISABLE_DATE)
					VALUES ($sponsor_id,1,NOW(),DATE_ADD(str_to_date('$endDate','%d/%m/%Y'), INTERVAL 1 DAY)
							,DATE_ADD(str_to_date('$endDate','%d/%m/%Y')
							, INTERVAL $nbDaySponsor+1 DAY))";
			
			$donnees = mysql_query($req);
		}
				
	}
	
	function multipleInscription($req, $nbArg, $args)
	{
		$id = 0;
		
		// Remplacement des valeurs dans la requêtes
		for ( $i=0 ; $i < $nbArg ; $i++ )
		{
			$req[0] = str_replace('?'.$i.'?',$args[$i],$req[0]);
			$req[6] = str_replace('?'.$i.'?',$args[$i],$req[6]);
		}
		
		//Requête de recherche de la franchise
		$donnees = mysql_query($req[6]);
		$result = mysql_fetch_row($donnees);
		$franchise = $result[0];
		
		//Requête de recherche par email
		$donnees = mysql_query($req[0]);
		$nb_rows_selected = mysql_num_rows($donnees);
		
		//Inscription du membre
		if ($nb_rows_selected == 1)
		{
			$result = mysql_fetch_row($donnees);
			$id = $result[0];
			
			//Inscription du membre
			$req[1] = str_replace('?id?',$id,$req[1]);
			// Remplacement des valeurs dans la requêtes
			for ( $i=0 ; $i < $nbArg ; $i++ )
			{
				$req[1] = str_replace('?'.$i.'?',$args[$i],$req[1]);
			}
			
			$donnees = mysql_query($req[1]);
			
			exit;
		}
		
		// Remplacement des valeurs dans la requêtes
		for ( $i=0 ; $i < $nbArg ; $i++ )
		{
			$req[2] = str_replace('?'.$i.'?',$args[$i],$req[2]);
		}
		
		//Requête de recherche par nom prenom
		$donnees = mysql_query($req[2]);
		$nb_rows_selected = mysql_num_rows($donnees);
		
		if ($nb_rows_selected > 1)
		{
			//Affichage des entêtes
			$nbFields = mysql_num_fields($donnees);
			for ( $i = 0 ; $i < $nbFields ; $i++)
			{
				if ( $i != 0 )
					echo '<SEP>';
				echo mysql_field_name($donnees,$i);
			}
			echo '<ENTETE>';
			
			//Affichage des lignes
			while ($result = mysql_fetch_row($donnees))
			{
				for ( $i = 0 ; $i < $nbFields ; $i++)
				{
					if ( $i != 0 )
						echo '<SEP>';
					echo $result[$i];
				}
				echo '<LIGNE>';
			}
			exit;
		}
		else if ($nb_rows_selected == 1) //Inscription du membre
		{
			$result = mysql_fetch_row($donnees);
			$id = $result[0];
			
			//Inscription du membre
			$req[1] = str_replace('?id?',$id,$req[1]);
			// Remplacement des valeurs dans la requêtes
			for ( $i=0 ; $i < $nbArg ; $i++ )
			{
				$req[1] = str_replace('?'.$i.'?',$args[$i],$req[1]);
			}
			
			$donnees = mysql_query($req[1]);
			
			exit;
		}
		
		//Creation du membre
		// Remplacement des valeurs dans la requêtes
		for ( $i=0 ; $i < $nbArg ; $i++ )
		{
			$req[3] = str_replace('?'.$i.'?',$args[$i],$req[3]);
			$req[4] = str_replace('?'.$i.'?',$args[$i],$req[4]);
			$req[5] = str_replace('?'.$i.'?',$args[$i],$req[5]);
		}
		
		$req[3] = str_replace('?franchise?',$franchise,$req[3]);
		
		$donnees = mysql_query($req[3]);
		$id = mysql_insert_id();
		
		$req[4] = str_replace('?id?',$id,$req[4]);
		$req[5] = str_replace('?id?',$id,$req[5]);
		$req[7] = str_replace('?id?',$id,$req[7]);
		$donnees = mysql_query($req[4]);
		$donnees = mysql_query($req[5]);
		$donnees = mysql_query($req[7]);
		
		//Inscription du membre
		$req[1] = str_replace('?id?',$id,$req[1]);
		// Remplacement des valeurs dans la requêtes
		for ( $i=0 ; $i < $nbArg ; $i++ )
		{
			$req[1] = str_replace('?'.$i.'?',$args[$i],$req[1]);
		}
		
		$donnees = mysql_query($req[1]);
	}
	
	function computeFidSponsor($sponsor_id, $member, $nbMonth)
	{
		$req = "INSERT INTO TB19_FID_POINTS (ID_MEMBER, EVENT_DATE, ID_FID_TYPE, NB, COMMENT, ID_REF_OPERATION)
				VALUES ($sponsor_id,NOW(),3,(SELECT (SELECT RATIO * $nbMonth FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = $nbMonth) * VALUE FROM TB18_FID_TYPE WHERE ID = 3),(SELECT CONCAT('Parrainage de ',FIRSTNAME, ' ',NAME) FROM TB03_MEMBER WHERE ID = $member),$member)";
		
		$data_verif = mysql_query($req);
		
		$req = "UPDATE TB20_RECAP T20
				SET NB_FID_POINTS = NB_FID_POINTS + (SELECT (SELECT RATIO * $nbMonth FROM TB18_FID_ABONNEMENT_RATIO WHERE NB_MONTH = $nbMonth) * VALUE FROM TB18_FID_TYPE WHERE ID = 3)
				WHERE ID_MEMBER = $sponsor_id";
				
		$data_verif = mysql_query($req);
	}
?>