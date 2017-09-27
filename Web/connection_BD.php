<?php 
	$bdd = @mysql_connect('localhost', 'root', '') or die('Erreur : '.mysql_error());
	mysql_set_charset('utf8', $bdd);
	if (!mysql_select_db("pokersphbase")) {
		echo "Impossible de sélectionner la base pokersphbase : " . mysql_error();
		exit;
	}
?>