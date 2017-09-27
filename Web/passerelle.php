<?php
	//Connection à la BDD
	include('./connection_BD.php');
	
	//Inclusion des fonctions
	include('./fonctions.php');
	
	// Récupération des arguments
	$_POST['requete'] = strip_tags($_POST['requete']);
	$_POST['type'] = strip_tags($_POST['type']);
	$_POST['nbArg'] = strip_tags($_POST['nbArg']);
	$franchise = 0;
	if (isset($_POST['franchise']))
		$franchise = strip_tags($_POST['franchise']);
	//debug
	// $_POST['requete'] = strip_tags($_GET['requete']);
	// $_POST['type'] = strip_tags($_GET['type']);
	// $_POST['nbArg'] = strip_tags($_GET['nbArg']);
	
	$args[] = array();
	
	for ( $i=0 ; $i < $_POST['nbArg'] ; $i++ )
	{
		$args[$i] = strip_tags($_POST['arg' . $i]);
		//debug
		// $args[$i] = strip_tags($_GET['arg' . $i]);
	}
	
	switch ($_POST['type'])
	{
		case "SELECT":
			if ($_POST['requete'] == 2 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 6 && $_POST['nbArg'] == 4 && $args[3] > 0)
				$franchise = $args[3];
			if ($_POST['requete'] == 9 && $_POST['nbArg'] == 2 && $args[1] > 0)
				$franchise = $args[1];
			if ($_POST['requete'] == 10 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 12 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 13 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 14 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 15 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 17 && $_POST['nbArg'] == 3 && $args[2] > 0)
				$franchise = $args[2];
			if ($_POST['requete'] == 18 && $_POST['nbArg'] == 1 && $args[0] > 0)
				$franchise = $args[0];
			if ($_POST['requete'] == 21 && $_POST['nbArg'] == 3 && $args[2] > 0)
				$franchise = $args[2];
			if ($_POST['requete'] == 32 && $franchise ==  0)
				$franchise = $args[0];
			$req = select($_POST['requete'], $franchise);
			break;
		case "INSERT":
			$req = insert($_POST['requete'], $franchise);
			if ($_POST['requete'] == 1 && $args[14] > 0) // Bonus sponsor
			{
				$reqTemp[] = insert(15, $franchise);
				$req = array_merge($req, $reqTemp[0]);
			}
			if ($_POST['requete'] == 16)
			{
				multipleInscription($req, $_POST['nbArg'], $args);
				exit;
			}
			break;
		case "UPDATE":
			$req = update($_POST['requete'], $franchise);
			if ($_POST['requete'] == 1 && $_POST['nbArg'] == 15)
			{
				$reqTemp[] = insert(13, $franchise);
				$req = array_merge($req, $reqTemp[0]);
			}
			else if ($_POST['requete'] == 6 || $_POST['requete'] == 7) 
			{
				if ($args[3] > 0) // Gain Fid tournoi
				{
					$reqTemp[] = insert(23, $franchise);
					$req = array_merge($req, $reqTemp[0]);
				}
				
				if ($_POST['nbArg'] > 5) // Dotation tournoi
				{
					$i = 5;
					// Mise à jour spéciale car plusieurs dotation possible
					while ( $i < $_POST['nbArg'] )
					{
						$reqTemp2[] = insert(26, $franchise);
						$reqTemp2[0] = str_replace('?a?',$args[$i++],$reqTemp2[0]);
						$reqTemp2[0] = str_replace('?b?',$args[$i++],$reqTemp2[0]);
						$reqTemp2[0] = str_replace('?c?',$args[$i++],$reqTemp2[0]);
						$req = array_merge($req, $reqTemp2[0]);
						unset($reqTemp2);
					}
				}
			}
			else if ($_POST['requete'] == 8)
			{
				$reqTemp[] = select(7, $franchise);
				$req = array_merge($req, $reqTemp[0]);
				$_POST['type'] = "SELECT";
			}
			
			break;
		case "DELETE":
			
			$req = delete($_POST['requete'], $franchise);
			
			if ($_POST['requete'] == 5)
			{
				$reqTemp[] = select(7, $franchise);
				$req = array_merge($req, $reqTemp[0]);
				$_POST['type'] = "SELECT";
			}
			break;
		case "GET_FILE":
			switch ($_POST['requete'])
			{
				case 1:
					getInfosPhoto($args[0]);
					echo "<GET_PHOTO>";
					echo getPhoto($args[0]);
					exit;
					break;
			}
			
			break;
		
		case "SET_FILE":
			switch ($_POST['requete'])
			{
				case 1:
					echo "<PUT_PHOTO>";
					savePhoto($args[0], $args[1]);
					exit;
					break;
			}
			
			break;
		case "ARRIVE":
			if ($_POST['nbArg'] == 3 && $args[2] > 0)
				$franchise = $args[2];
			arrive($args[0], $args[1], $franchise);
			exit;
			break;
	}
	
	$msg = "";
	
	// On exécute chaque requête
	$id = 0;
	$id2 = 0;
	for ($r=0 ; $r < sizeof($req) ; $r++)
	{
		// S'il y a plusieurs requêtes, on récupère une fois l'id inséré
		if ($r == 1 && ($_POST['type'] != "UPDATE" || $_POST['requete'] != 13))
			$id = mysql_insert_id();
		
		if ($_POST['type'] == "INSERT" && $r == 2 && $_POST['requete'] == 1)
			$id2 = mysql_insert_id();
		
		// On remplace l'id si nécessaire
		if ($r > 0) {
			$req[$r] = str_replace('?id?',$id,$req[$r]);
			$req[$r] = str_replace('?id2?',$id2,$req[$r]);
		}
	
		// Remplacement des valeurs dans la requêtes
		for ( $i=0 ; $i < $_POST['nbArg'] ; $i++ )
		{
			$req[$r] = str_replace('?'.$i.'?',$args[$i],$req[$r]);
		}
		
		if ($_POST['type'] == "INSERT" ) 
		{
			if ($_POST['requete'] == 1 && $r == 3) //INSERT INTO TB13_MEMBER_CARD
			{
				if ($args[13] == "")
				{
					continue;
				}
				$req[$r] = str_replace("''","null",$req[$r]);
			}
			if ($_POST['requete'] == 1 && $r == 4) //INSERT INTO TB19_FID_POINTS
			{
				if ($args[16] == "0")
				{
					continue;
				}
			}
			if ($_POST['requete'] == 2 && $r == 1) //INSERT INTO TB19_FID_POINTS
			{
				if ($args[5] == "0")
				{
					continue;
				}
			}
			else if ($_POST['requete'] == 21)
			{
				$req[$r] = str_replace("''","null",$req[$r]);
			}
		}
		
		if ($_POST['type'] == "UPDATE" && $_POST['requete'] == 1 && $r == 2 )
		{
			$req[$r] = str_replace("''","null",$req[$r]);
		}
	
		$donnees = mysql_query($req[$r]);
		$nb_rows_affected = mysql_affected_rows();
		
		if (!$donnees) {
			$display_error = 1;
			echo '<ERREUR>';
			switch ($_POST['type'])
			{
				case "INSERT":
					switch ($_POST['requete'])
					{
						case 1:
							if ($r == 0 && $nb_rows_affected == -1) //Contrainte d'unicité
							{
								echo "Cet adhérent existe déjà ! (même nom, prénom et email)";
								exit;
							}
							else if ($r == 3 && $nb_rows_affected == -1) //Contrainte d'unicité
							{
								//echo '<CONTINUE>';
								//echo "L'adhérent a été créé, mais le numéro de carte membre spécifié est déjà attribué ! Donner un autre numéro.";
								$msg = "<CONTINUE>L'adhérent a été créé, mais le numéro de carte membre spécifié est déjà attribué ! Donner un autre numéro.";
								$display_error = 0;
								//exit;
							}
							break;
						case 8:
							if (mysql_errno() == 1062) //Doublon
							{
								echo 'Ce membre est déjà inscrit à ce tournoi.';
								exit;
							}
							break;
					}
					break;
				case "UPDATE":
					switch ($_POST['requete'])
					{
						case 1:
							if ($r == 2 && $nb_rows_affected == -1) //Contrainte d'unicité
							{
								echo 'Ce numéro de carte membre est déjà attribué !';
								exit;
							}
							break;
					}
					break;
				
			}
			if ($display_error)
			{
				echo 'Impossible d\'exécuter la requête : ' . mysql_error();
				//echo $req[$r];
				exit;
			}
		} else {
			switch ($_POST['type'])
			{
				case "UPDATE":
					switch ($_POST['requete'])
					{
						case 1:
							if ($r == 2 && $nb_rows_affected == 0) //Contrainte d'unicité
							{
								echo '<ERREUR>';
								echo 'Ce numéro de carte membre est déjà attribué !';
								exit;
							}
							break;
							
						case 13:
							$result = mysql_fetch_row($donnees);
							$id = $result[0];
							break;
					}
					break;
			}
		}
		
	}
	
	// Si on ne fait pas un select, on n'affiche un message
	if ($_POST['type'] != "SELECT")
	{
		if ($_POST['type'] != "UPDATE" || $_POST['requete'] != 5 )
		{
			echo '<MESSAGE>';
			switch ($_POST['type'])
			{
				case "INSERT":
					switch ($_POST['requete'])
					{
						case 1:
							if ($_POST['nbArg'] >= 19)
								savePhoto($id, $args[18]);
							
							if ($args[14] > 0 && $args[11] >= 1 && $args[11] <= 4) //Compute sponsor Bonus
							{
								computeSponsorBonus($id, $args[14], $args[11], $args[16]);
							}
							if ($msg == "")
								echo 'Adhérent créé avec succès.';
							else
								echo $msg;
							break;
						
						case 2:
							if ($args[1] >= 1 && $args[1] <= 4) //Abonnement payant
							{
								needSponsorBonus($args[0],$args[1],$args[5]);
							}
							echo 'Adhésion créé avec succès.';
							break;
							
						case 3:
							echo 'Retrait effectué avec succès.';
							break;
							
						case 4:
							echo 'Dépôt effectué avec succès.';
							break;
						
						case 5:
							echo 'Enchère effectuée avec succès.';
							break;
						
						case 6:
							echo 'Opération effectuée avec succès.';
							break;
							
						case 7:
							echo 'Tournoi créé avec succès.';
							break;
						
						case 8:
							echo 'Inscription effectuée.';
							break;
						
						case 9:
							echo 'Tournoi validé, les inscriptions ont été décomptées.';
							break;
						
						case 10:
							echo 'Tournoi annulé, les inscriptions ont été recréditées.';
							break;
							
						case 11:
							echo 'Le bonus mensuel adhésion a été crédité.';
							break;
							
						case 12:
							$id = mysql_insert_id();
							echo "Template de tournoi créé avec succès.";
							echo "<MESSAGE><ID>$id";
							break;
						case 14:
							echo 'Carte attribuée avec succès.';
							break;
						
						case 17:
							echo 'Adhérent créé avec succès.';
							break;
							
						case 18:
							echo 'Inscription au tournoi lié effectuée.';
							break;
							
						case 19:
							echo 'Club ajouté avec succès.';
							break;
							
						case 20:
							$id = mysql_insert_id();
							echo 'Statut de fidélisation créé avec succès.';
							echo "<MESSAGE><ID>$id";
							break;
							
						case 21:
							echo 'Opération de fidélisation créée avec succès.';
							break;
							
						case 22:
							echo 'Euros chargés avec succès.';
							break;

						case 24:
							echo 'Type de dotation créé avec succès.';
							break;
							
						case 25:
							echo 'Description de dotation créée avec succès.';
							break;
					}
					break;
				case "UPDATE":
					switch ($_POST['requete'])
					{
						case 1:
							echo 'Adhérent mis à jour avec succès.';
							break;
						
						case 2:
							echo 'Adhésion mis à jour avec succès.';
							break;
						
						case 3:
							echo 'Opération mise à jour avec succès.';
							break;
							
						case 4:
							echo 'Tournoi mis à jour avec succès.';
							break;
							
						case 6:
							echo 'Membre sortie avec succès.';
							break;
						
						case 7:
							echo 'Membre sortie avec succès.';
							break;
						
						case 9:
							echo 'Template de tournoi mis à jour avec succès.';
							break;
						
						case 10:
							echo 'Bonus parrain mis à jour avec succès.';
							break;
							
						case 11:
							echo 'Données envoyées à RankingHero avec succès.';
							break;
							
						case 12:
							echo 'Franchise modifiée avec succès.';
							break;
							
						case 13:
							echo 'Tournoi finalisé avec succès.';
							break;
							
						case 14:
							echo 'Club modifié avec succès.';
							break;
							
						case 15:
							echo 'Statut de fidélisation mis à jour avec succès.';
							break;
							
						case 16:
							echo 'Types de fidélisation mis à jour avec succès.';
							break;
							
						case 17:
							echo 'Opération de fidélisation mis à jour avec succès.';
							break;
							
						case 18:
							echo 'Type de dotation mis à jour avec succès.';
							break;
							
						case 19:
							echo 'Description de dotation mise à jour avec succès.';
							break;
					}
					break;
				case "DELETE":
					switch ($_POST['requete'])
					{
						case 1:
							echo 'Adhérent supprimé.';
							break;
						
						case 2:
							echo 'Adhésion supprimé.';
							break;
						
						case 3:
							echo 'Opération supprimée.';
							break;
							
						case 4:
							echo 'Tournoi supprimé.';
							break;
						
						case 5:
							echo 'Membre désinscrit.';
							break;
							
						case 6:
							echo 'Template de tournoi supprimé.';
							break;
							
						case 7:
							echo 'Club supprimé.';
							break;
							
						case 8:
							echo 'Statut de fidélisation supprimé.';
							break;
							
						case 9:
							echo 'Opération supprimée.';
							break;
							
						case 10:
							echo 'Type de dotation supprimé.';
							break;
							
						case 11:
							echo 'Description de dotation supprimée.';
							break;
					}
					break;
			}
		}
		exit;
	}
	
	/*if ($_POST['requete'] == 15)
	{
		chdir('photos');
		while ($result = mysql_fetch_row($donnees))
		{
			$filename = $result[0].'.jpg';
			if (is_writable($filename))
			{
				$file = fopen($filename,'ab');
				fputs($file,$result[1]);
				fclose($file);
			}
			else
				echo 'problème droit écriture';
		}
		exit;
	}*/
	
	
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
?>